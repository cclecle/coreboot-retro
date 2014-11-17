/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2013-2014 Sage Electronic Engineering, LLC.
 * Copyright (C) 2014 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <types.h>
#include <string.h>
#include <console/console.h>
#include <bootstate.h>
#include <cbmem.h>
#include <device/device.h>
#include <device/pci_def.h>
#include <baytrail/pci_devs.h>
#include <drivers/intel/fsp/fsp_util.h>
#include "../chip.h"
#include <arch/io.h>
#include <baytrail/reset.h>
#include <baytrail/pmc.h>
#include <baytrail/acpi.h>
#include <baytrail/iomap.h>

#ifdef __PRE_RAM__
#include <baytrail/romstage.h>
#endif

#ifdef __PRE_RAM__

/* Copy the default UPD region and settings to a buffer for modification */
static void GetUpdDefaultFromFsp (FSP_INFO_HEADER *FspInfo, UPD_DATA_REGION   *UpdData)
{
	VPD_DATA_REGION *VpdDataRgnPtr;
	UPD_DATA_REGION *UpdDataRgnPtr;
	VpdDataRgnPtr = (VPD_DATA_REGION *)(UINT32)(FspInfo->CfgRegionOffset  + FspInfo->ImageBase);
	UpdDataRgnPtr = (UPD_DATA_REGION *)(UINT32)(VpdDataRgnPtr->PcdUpdRegionOffset + FspInfo->ImageBase);
	memcpy((void*)UpdData, (void*)UpdDataRgnPtr, sizeof(UPD_DATA_REGION));
}

/* default to just enabling HDMI audio */
const PCH_AZALIA_CONFIG mAzaliaConfig = {
	.Pme = 1,
	.DS = 1,
	.DA = 0,
	.HdmiCodec = 1,
	.AzaliaVCi = 1,
	.Rsvdbits = 0,
	.AzaliaVerbTableNum = 0,
	.AzaliaVerbTable = NULL,
	.ResetWaitTimer = 300
};

typedef struct soc_intel_fsp_baytrail_config config_t;

/**
 * Update the UPD data based on values from devicetree.cb
 *
 * @param UpdData Pointer to the UPD Data structure
 */
static void ConfigureDefaultUpdData(FSP_INFO_HEADER *FspInfo, UPD_DATA_REGION *UpdData)
{
	ROMSTAGE_CONST struct device *dev;
	ROMSTAGE_CONST config_t *config;
	printk(BIOS_DEBUG, "Configure Default UPD Data\n");

	dev = dev_find_slot(0, SOC_DEV_FUNC);
	config = dev->chip_info;

	/* Set up default verb tables - Just HDMI audio */
	UpdData->AzaliaConfigPtr = (UINT32)&mAzaliaConfig;

	/* Set SPD addresses */
	UPD_SPD_CHECK(PcdMrcInitSPDAddr1);
	UPD_SPD_CHECK(PcdMrcInitSPDAddr2);

	UPD_DEFAULT_CHECK(PcdSataMode);
	UPD_DEFAULT_CHECK(PcdLpssSioEnablePciMode);
	UPD_DEFAULT_CHECK(PcdMrcInitMmioSize);
	UPD_DEFAULT_CHECK(PcdIgdDvmt50PreAlloc);
	UPD_DEFAULT_CHECK(PcdApertureSize);
	UPD_DEFAULT_CHECK(PcdGttSize);
	UPD_DEFAULT_CHECK(SerialDebugPortAddress);
	UPD_DEFAULT_CHECK(SerialDebugPortType);
	UPD_DEFAULT_CHECK(PcdMrcDebugMsg);
	UPD_DEFAULT_CHECK(PcdSccEnablePciMode);
	UPD_DEFAULT_CHECK(IgdRenderStandby);
	UPD_DEFAULT_CHECK(TxeUmaEnable);

	if ((config->PcdeMMCBootMode != EMMC_USE_DEFAULT) ||
			(config->PcdeMMCBootMode != EMMC_FOLLOWS_DEVICETREE))
		UpdData->PcdeMMCBootMode = config->PcdeMMCBootMode;

	if (config->PcdMrcInitTsegSize != TSEG_SIZE_DEFAULT)
		UpdData->PcdMrcInitTsegSize = config->PcdMrcInitTsegSize - 1;

	printk(BIOS_DEBUG, "GTT Size:\t\t%d MB\n", UpdData->PcdGttSize);
	printk(BIOS_DEBUG, "Tseg Size:\t\t%d MB\n", UpdData->PcdMrcInitTsegSize);
	printk(BIOS_DEBUG, "Aperture Size:\t\t%d MB\n",
		APERTURE_SIZE_BASE << UpdData->PcdApertureSize);
	printk(BIOS_DEBUG, "IGD Memory Size:\t%d MB\n",
		UpdData->PcdIgdDvmt50PreAlloc * IGD_MEMSIZE_MULTIPLIER);
	printk(BIOS_DEBUG, "MMIO Size:\t\t%d MB\n", UpdData->PcdMrcInitMmioSize);

	/* Advance dev to PCI device 0.0 */
	for (dev = &dev_root; dev; dev = dev_find_next_pci_device(dev)){
		if (dev->path.type != DEVICE_PATH_PCI)
			continue;
		if (dev->path.pci.devfn == PCI_DEVFN(0x0,0))
			break;
	}

	/*
	 * Loop through all the SOC devices in the devicetree
	 *  enabling and disabling them as requested.
	 */
	for (; dev; dev = dev->sibling) {

		if (dev->path.type != DEVICE_PATH_PCI)
			continue;

		switch (dev->path.pci.devfn) {
			case MIPI_DEV_FUNC:	/* Camera / Image Signal Processing */
				if (FspInfo->ImageRevision >= FSP_GOLD3_REV_ID) {
					UpdData->ISPEnable = dev->enabled;
				} else {
					/* Gold2 and earlier FSP: ISPEnable is the filed	*/
					/* next to PcdGttSize in UPD_DATA_REGION struct		*/
					*(&(UpdData->PcdGttSize)+sizeof(UINT8)) = dev->enabled;
					printk (BIOS_DEBUG,
						"Baytrail Gold2 or earlier FSP, adjust ISPEnable offset.\n");
				}
				printk(BIOS_DEBUG, "MIPI/ISP:\t\t%s\n",
						UpdData->PcdEnableSdio?"Enabled":"Disabled");
				break;
			case EMMC_DEV_FUNC: /* EMMC 4.1*/
				if ((dev->enabled) &&
						(config->PcdeMMCBootMode == EMMC_FOLLOWS_DEVICETREE))
					UpdData->PcdeMMCBootMode = EMMC_4_1 - EMMC_DISABLED;
				break;
			case SDIO_DEV_FUNC:
				UpdData->PcdEnableSdio = dev->enabled;
				printk(BIOS_DEBUG, "Sdio:\t\t\t%s\n",
						UpdData->PcdEnableSdio?"Enabled":"Disabled");
				break;
			case SD_DEV_FUNC:
				UpdData->PcdEnableSdcard = dev->enabled;
				printk(BIOS_DEBUG, "Sdcard:\t\t\t%s\n",
						UpdData->PcdEnableSdcard?"Enabled":"Disabled");
				break;
			case SATA_DEV_FUNC:
				UpdData->PcdEnableSata = dev->enabled;
				printk(BIOS_DEBUG, "Sata:\t\t\t%s\n",
						UpdData->PcdEnableSata?"Enabled":"Disabled");
				if (UpdData->PcdEnableSata)
					printk(BIOS_DEBUG, "SATA Mode:\t\t%s\n",
						UpdData->PcdSataMode?"AHCI":"IDE");
				break;
			case XHCI_DEV_FUNC:
				UpdData->PcdEnableXhci = dev->enabled;
				break;
			case LPE_DEV_FUNC:
				if (dev->enabled && config->LpeAcpiModeEnable ==
						LPE_ACPI_MODE_ENABLED)
					UpdData->PcdEnableLpe = LPE_ACPI_MODE_ENABLED;
				else
				UpdData->PcdEnableLpe = dev->enabled;
				printk(BIOS_DEBUG, "Lpe:\t\t\t%s\n",
						UpdData->PcdEnableLpe?"Enabled":"Disabled");
				printk(BIOS_DEBUG, "Lpe mode:\t\t%s\n",
						UpdData->PcdEnableLpe == LPE_ACPI_MODE_ENABLED?
						"ACPI":"PCI");
				break;
			case MMC45_DEV_FUNC: /* MMC 4.5*/
				if ((dev->enabled) &&
						(config->PcdeMMCBootMode == EMMC_FOLLOWS_DEVICETREE))
					UpdData->PcdeMMCBootMode = EMMC_4_5 - EMMC_DISABLED;
				break;
			case SIO_DMA1_DEV_FUNC:
				UpdData->PcdEnableDma0 = dev->enabled;
				printk(BIOS_DEBUG, "SIO Dma 0:\t\t%s\n",
						UpdData->PcdEnableDma0?"Enabled":"Disabled");
				break;
			case I2C1_DEV_FUNC:
				UpdData->PcdEnableI2C0 = dev->enabled;
				printk(BIOS_DEBUG, "SIO I2C0:\t\t%s\n",
						UpdData->PcdEnableI2C0?"Enabled":"Disabled");
				break;
			case I2C2_DEV_FUNC:
				UpdData->PcdEnableI2C1 = dev->enabled;
				printk(BIOS_DEBUG, "SIO I2C1:\t\t%s\n",
						UpdData->PcdEnableI2C1?"Enabled":"Disabled");
				break;
			case I2C3_DEV_FUNC:
				UpdData->PcdEnableI2C2 = dev->enabled;
				printk(BIOS_DEBUG, "SIO I2C2:\t\t%s\n",
						UpdData->PcdEnableI2C2?"Enabled":"Disabled");
				break;
			case I2C4_DEV_FUNC:
				UpdData->PcdEnableI2C3 = dev->enabled;
				printk(BIOS_DEBUG, "SIO I2C3:\t\t%s\n",
						UpdData->PcdEnableI2C3?"Enabled":"Disabled");
				break;
			case I2C5_DEV_FUNC:
				UpdData->PcdEnableI2C4 = dev->enabled;
				printk(BIOS_DEBUG, "SIO I2C4:\t\t%s\n",
						UpdData->PcdEnableI2C4?"Enabled":"Disabled");
				break;
			case I2C6_DEV_FUNC:
				UpdData->PcdEnableI2C5 = dev->enabled;
				printk(BIOS_DEBUG, "SIO I2C5:\t\t%s\n",
						UpdData->PcdEnableI2C5?"Enabled":"Disabled");
				break;
			case I2C7_DEV_FUNC:
				UpdData->PcdEnableI2C6 = dev->enabled;
				printk(BIOS_DEBUG, "SIO I2C6:\t\t%s\n",
						UpdData->PcdEnableI2C6?"Enabled":"Disabled");
				break;
			case TXE_DEV_FUNC: /* TXE */
				break;
			case HDA_DEV_FUNC:
				if (config->AzaliaAutoEnable) {
					UpdData->PcdEnableAzalia = 2;
					printk(BIOS_DEBUG, "Azalia:\t\t\tAuto\n");
				} else {
					UpdData->PcdEnableAzalia = dev->enabled;
					printk(BIOS_DEBUG, "Azalia:\t\t\t%s\n",
						UpdData->PcdEnableAzalia?"Enabled":"Disabled");
				}
				break;
			case PCIE_PORT1_DEV_FUNC:
			case PCIE_PORT2_DEV_FUNC:
			case PCIE_PORT3_DEV_FUNC:
			case PCIE_PORT4_DEV_FUNC:
				break;
			case EHCI_DEV_FUNC:
				UpdData->PcdEnableXhci = !(dev->enabled);
				break;
			case SIO_DMA2_DEV_FUNC:
				UpdData->PcdEnableDma1 = dev->enabled;
				printk(BIOS_DEBUG, "SIO Dma1:\t\t%s\n",
						UpdData->PcdEnableDma1?"Enabled":"Disabled");
				break;
			case PWM1_DEV_FUNC:
				UpdData->PcdEnablePwm0 = dev->enabled;
				printk(BIOS_DEBUG, "Pwm0\t\t\t%s\n",
						UpdData->PcdEnablePwm0?"Enabled":"Disabled");
				break;
			case PWM2_DEV_FUNC:
				UpdData->PcdEnablePwm1 = dev->enabled;
				printk(BIOS_DEBUG, "Pwm1:\t\t\t%s\n",
						UpdData->PcdEnablePwm1?"Enabled":"Disabled");
				break;
			case HSUART1_DEV_FUNC:
				UpdData->PcdEnableHsuart0 = dev->enabled;
				printk(BIOS_DEBUG, "Hsuart0:\t\t%s\n",
						UpdData->PcdEnableHsuart0?"Enabled":"Disabled");
				break;
			case HSUART2_DEV_FUNC:
				UpdData->PcdEnableHsuart1 = dev->enabled;
				printk(BIOS_DEBUG, "Hsuart1:\t\t%s\n",
						UpdData->PcdEnableHsuart1?"Enabled":"Disabled");
				break;
			case SPI_DEV_FUNC:
				UpdData->PcdEnableSpi = dev->enabled;
				printk(BIOS_DEBUG, "Spi:\t\t\t%s\n",
						UpdData->PcdEnableSpi?"Enabled":"Disabled");
				break;
			case LPC_DEV_FUNC: /* LPC */
				break;
			case SMBUS_DEV_FUNC:
				break;
		}
	}

	if(UpdData->PcdeMMCBootMode == EMMC_AUTO - EMMC_DISABLED) {
		printk(BIOS_DEBUG, "eMMC Mode:\t\tAuto");
	} else {
		printk(BIOS_DEBUG, "eMMC 4.1:\t\t%s\n",
			UpdData->PcdeMMCBootMode == EMMC_4_1 - EMMC_DISABLED?
			"Enabled":"Disabled");
		printk(BIOS_DEBUG, "eMMC 4.5:\t\t%s\n",
			UpdData->PcdeMMCBootMode == EMMC_4_5 - EMMC_DISABLED?
			"Enabled":"Disabled");
	}
	printk(BIOS_DEBUG, "Xhci:\t\t\t%s\n",
		UpdData->PcdEnableXhci?"Enabled":"Disabled");

	/* set memory down parameters */
	if (config->EnableMemoryDown != MEMORY_DOWN_DEFAULT) {
		UpdData->PcdMemoryParameters.EnableMemoryDown
			= config->EnableMemoryDown - MEMORY_DOWN_DISABLE;

		if (config->DRAMSpeed != DRAM_SPEED_DEFAULT) {
			UpdData->PcdMemoryParameters.DRAMSpeed
			= config->DRAMSpeed - DRAM_SPEED_800MHZ;
		}
		if (config->DRAMType != DRAM_TYPE_DEFAULT) {
			UpdData->PcdMemoryParameters.DRAMType
			= config->DRAMType - DRAM_TYPE_DDR3;
		}
		if (config->DIMM0Enable != DIMM0_ENABLE_DEFAULT) {
			UpdData->PcdMemoryParameters.DIMM0Enable
			= config->DIMM0Enable - DIMM0_DISABLE;
		}
		if (config->DIMM1Enable != DIMM1_ENABLE_DEFAULT) {
			UpdData->PcdMemoryParameters.DIMM1Enable
			= config->DIMM1Enable - DIMM1_DISABLE;
		}
		if (config->DIMMDWidth != DIMM_DWIDTH_DEFAULT) {
			UpdData->PcdMemoryParameters.DIMMDWidth
			= config->DIMMDWidth - DIMM_DWIDTH_X8;
		}
		if (config->DIMMDensity != DIMM_DENSITY_DEFAULT) {
			UpdData->PcdMemoryParameters.DIMMDensity
			= config->DIMMDensity - DIMM_DENSITY_1G_BIT;
		}
		if (config->DIMMBusWidth != DIMM_BUS_WIDTH_DEFAULT) {
			UpdData->PcdMemoryParameters.DIMMBusWidth
			= config->DIMMBusWidth - DIMM_BUS_WIDTH_8BIT;
		}
		if (config->DIMMSides != DIMM_SIDES_DEFAULT) {
			UpdData->PcdMemoryParameters.DIMMSides
			= config->DIMMSides - DIMM_SIDES_1RANK;
		}
		if (config->DIMMtCL != DIMM_TCL_DEFAULT)
			UpdData->PcdMemoryParameters.DIMMtCL		= config->DIMMtCL;
		if (config->DIMMtRPtRCD != DIMM_TRP_TRCD_DEFAULT)
			UpdData->PcdMemoryParameters.DIMMtRPtRCD	= config->DIMMtRPtRCD;
		if (config->DIMMtWR != DIMM_TWR_DEFAULT)
			UpdData->PcdMemoryParameters.DIMMtWR		= config->DIMMtWR;
		if (config->DIMMtWTR != DIMM_TWTR_DEFAULT)
			UpdData->PcdMemoryParameters.DIMMtWTR		= config->DIMMtWTR;
		if (config->DIMMtRRD != DIMM_TRRD_DEFAULT)
			UpdData->PcdMemoryParameters.DIMMtRRD		= config->DIMMtRRD;
		if (config->DIMMtRTP != DIMM_TRTP_DEFAULT)
			UpdData->PcdMemoryParameters.DIMMtRTP		= config->DIMMtRTP;
		if (config->DIMMtFAW != DIMM_TFAW_DEFAULT)
			UpdData->PcdMemoryParameters.DIMMtFAW		= config->DIMMtFAW;

		printk (BIOS_DEBUG,
			"Memory Down Data Existed : %s\n"\
			"- Speed (0: 800, 1: 1066, 2: 1333, 3: 1600): %d\n"\
			"- Type  (0: DDR3, 1: DDR3L) : %d\n"\
			"- DIMM0        : %s\n"\
			"- DIMM1        : %s\n"\
			"- Width        : x%d\n"\
			"- Density      : %dGbit\n"
			"- BudWidth     : %dbit\n"\
			"- Rank #       : %d\n"\
			"- tCL          : %02X\n"\
			"- tRPtRCD      : %02X\n"\
			"- tWR          : %02X\n"\
			"- tWTR         : %02X\n"\
			"- tRRD         : %02X\n"\
			"- tRTP         : %02X\n"\
			"- tFAW         : %02X\n"
			, (UpdData->PcdMemoryParameters.EnableMemoryDown) ? "Enabled" : "Disabled"
			, UpdData->PcdMemoryParameters.DRAMSpeed
			, UpdData->PcdMemoryParameters.DRAMType
			, (UpdData->PcdMemoryParameters.DIMM0Enable) ? "Enabled" : "Disabled"
			, (UpdData->PcdMemoryParameters.DIMM1Enable) ? "Enabled" : "Disabled"
			, 8 << (UpdData->PcdMemoryParameters.DIMMDWidth)
			, 1 << (UpdData->PcdMemoryParameters.DIMMDensity)
			, 8 << (UpdData->PcdMemoryParameters.DIMMBusWidth)
			, (UpdData->PcdMemoryParameters.DIMMSides) + 1
			, UpdData->PcdMemoryParameters.DIMMtCL
			, UpdData->PcdMemoryParameters.DIMMtRPtRCD
			, UpdData->PcdMemoryParameters.DIMMtWR
			, UpdData->PcdMemoryParameters.DIMMtWTR
			, UpdData->PcdMemoryParameters.DIMMtRRD
			, UpdData->PcdMemoryParameters.DIMMtRTP
			, UpdData->PcdMemoryParameters.DIMMtFAW
			);
	}
}

/* Set up the Baytrail specific structures for the call into the FSP */
void chipset_fsp_early_init(FSP_INIT_PARAMS *pFspInitParams,
		FSP_INFO_HEADER *fsp_ptr)
{
	FSP_INIT_RT_BUFFER *pFspRtBuffer = pFspInitParams->RtBufferPtr;
	uint32_t prev_sleep_state;

	/* Get previous sleep state but don't clear */
	prev_sleep_state = chipset_prev_sleep_state(0);
	printk(BIOS_INFO, "prev_sleep_state = S%d\n", prev_sleep_state);

	/* Initialize the UPD Data */
	GetUpdDefaultFromFsp (fsp_ptr, pFspRtBuffer->Common.UpdDataRgnPtr);
	ConfigureDefaultUpdData(fsp_ptr, pFspRtBuffer->Common.UpdDataRgnPtr);
	pFspInitParams->NvsBufferPtr = NULL;

#if IS_ENABLED(CONFIG_ENABLE_MRC_CACHE)
	/* Find the fastboot cache that was saved in the ROM */
	pFspInitParams->NvsBufferPtr = find_and_set_fastboot_cache();
#endif

	if (prev_sleep_state == 3) {
		/* S3 resume */
		if ( pFspInitParams->NvsBufferPtr == NULL) {
			/* If waking from S3 and no cache then. */
			printk(BIOS_WARNING, "No MRC cache found in S3 resume path.\n");
			post_code(POST_RESUME_FAILURE);
			/* Clear Sleep Type */
			outl(inl(ACPI_BASE_ADDRESS + PM1_CNT) &
				~(SLP_TYP), ACPI_BASE_ADDRESS + PM1_CNT);
			/* Reboot */
			printk(BIOS_WARNING,"Rebooting..\n" );
			warm_reset();
			/* Should not reach here.. */
			die("Reboot System\n");
		}
		pFspRtBuffer->Common.BootMode = BOOT_ON_S3_RESUME;
	} else {
		/* Not S3 resume */
		pFspRtBuffer->Common.BootMode = BOOT_WITH_FULL_CONFIGURATION;
	}

	return;
}

/* The FSP returns here after the fsp_early_init call */
void ChipsetFspReturnPoint(EFI_STATUS Status,
		VOID *HobListPtr)
{
	if (Status == 0xFFFFFFFF) {
		warm_reset();
	}
	romstage_main_continue(Status, HobListPtr);
}

#endif	/* __PRE_RAM__ */
