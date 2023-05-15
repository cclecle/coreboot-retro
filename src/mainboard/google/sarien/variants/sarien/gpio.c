/* SPDX-License-Identifier: GPL-2.0-only */

#include <variant/gpio.h>
#include <vendorcode/google/chromeos/chromeos.h>

/* Pad configuration in ramstage */
static const struct pad_config gpio_table[] = {
/* RCIN# */		PAD_NC(GPP_A0, NONE),
/* ESPI_IO0 */
/* ESPI_IO1 */
/* ESPI_IO2 */
/* ESPI_IO3 */
/* ESPI_CS# */
/* SERIRQ */
/* PIRQA# */		PAD_NC(GPP_A7, NONE),
/* CLKRUN# */		PAD_NC(GPP_A8, NONE),
/* ESPI_CLK */
/* CLKOUT_LPC1 */	PAD_NC(GPP_A10, NONE),
/* PME# */		PAD_NC(GPP_A11, NONE),
/* BM_BUSY# */		PAD_NC(GPP_A12, NONE),

/* ESPI_RESET# */

/* SD_1P8_SEL */	PAD_NC(GPP_A16, NONE),
/* SD_PWR_EN# */	PAD_NC(GPP_A17, NONE),
/* ISH_GP0 */		PAD_NC(GPP_A18, NONE),
/* ISH_GP1 */		PAD_NC(GPP_A19, NONE),
/* ISH_GP2 */		PAD_NC(GPP_A20, NONE),
/* ISH_GP3 */		PAD_NC(GPP_A21, NONE),
/* ISH_GP4 */		PAD_NC(GPP_A22, NONE),
/* ISH_GP5 */		PAD_NC(GPP_A23, NONE),

/* CORE_VID0 */
/* CORE_VID1 */
/* VRALERT# */		PAD_NC(GPP_B2, NONE),
/* CPU_GP2 */		PAD_CFG_GPI_APIC(GPP_B3, NONE, PLTRST,
				 LEVEL, INVERT), /* TOUCHPAD_INTR# */
/* CPU_GP3 */		PAD_CFG_GPI(GPP_B4, NONE, DEEP), /* TOUCH_SCREEN_DET# */
/* SRCCLKREQ0# */	PAD_CFG_NF(GPP_B5, NONE, DEEP, NF1), /* CLKREQ_PCIE#0 */
/* SRCCLKREQ1# */	PAD_CFG_NF(GPP_B6, NONE, DEEP, NF1), /* CLKREQ_PCIE#1 */
/* SRCCLKREQ2# */	PAD_CFG_NF(GPP_B7, NONE, DEEP, NF1), /* CLKREQ_PCIE#2 */
/* SRCCLKREQ3# */	PAD_CFG_NF(GPP_B8, NONE, DEEP, NF1), /* CLKREQ_PCIE#3 */
/* SRCCLKREQ4# */	PAD_CFG_NF(GPP_B9, NONE, DEEP, NF1), /* CLKREQ_PCIE#4 */
/* SRCCLKREQ5# */	PAD_NC(GPP_B10, NONE),
/* EXT_PWR_GATE# */	PAD_CFG_GPO(GPP_B11, 0, DEEP), /* 3.3V_CAM_EN# */
/* SLP_S0# */		PAD_CFG_NF(GPP_B12, NONE, DEEP, NF1),
/* PLTRST# */		PAD_CFG_NF(GPP_B13, NONE, DEEP, NF1),
/* SPKR */		PAD_CFG_NF(GPP_B14, NONE, DEEP, NF1),
/* GSPI0_CS# */		PAD_NC(GPP_B15, NONE),
/* GSPI0_CLK */		PAD_NC(GPP_B16, NONE),
/* GSPI0_MISO */	PAD_NC(GPP_B17, NONE),
/* GSPI0_MOSI */	PAD_NC(GPP_B18, NONE),
/* GSPI1_CS# */		PAD_NC(GPP_B19, NONE), /* HDD_FALL_INT (nostuff) */
/* GSPI1_CLK */		PAD_NC(GPP_B20, NONE), /* TPM_PIRQ# (nostuff) */
/* GSPI1_MISO */	PAD_CFG_GPO(GPP_B21, 1, DEEP), /* PCH_3.3V_TS_EN */
/* GSPI1_MOSI */	PAD_NC(GPP_B22, NONE),
/* SML1ALERT# */	PAD_NC(GPP_B23, DN_20K),

/* SMBCLK */		PAD_CFG_NF(GPP_C0, NONE, DEEP, NF1), /* MEM_SMBCLK */
/* SMBDATA */		PAD_CFG_NF(GPP_C1, NONE, DEEP, NF1), /* MEM_SMBDATA */
/* SMBALERT# */		PAD_NC(GPP_C2, DN_20K),
/* SML0CLK */		PAD_CFG_NF(GPP_C3, NONE, DEEP, NF1), /* SML0_SMBCLK */
/* SML0DATA */		PAD_CFG_NF(GPP_C4, NONE, DEEP, NF1), /* SML0_SMBDATA */
/* SML0ALERT# */	PAD_NC(GPP_C5, DN_20K),
/* SM1CLK */		PAD_CFG_NF(GPP_C6, NONE, DEEP, NF1), /* SML1_SMBCLK */
/* SM1DATA */		PAD_CFG_NF(GPP_C7, NONE, DEEP, NF1), /* SML1_SMBDATA */
/* UART0_RXD */		PAD_NC(GPP_C8, NONE),
/* UART0_TXD */		PAD_NC(GPP_C9, NONE),
/* UART0_RTS# */	PAD_CFG_GPI(GPP_C10, NONE, DEEP), /* TYPEC_CON_SEL1 */
/* UART0_CTS# */	PAD_CFG_GPI(GPP_C11, NONE, DEEP), /* TYPEC_CON_SEL2 */
/* UART1_RXD */         PAD_CFG_GPI_SCI_LOW(GPP_C12, NONE, DEEP,
				    EDGE_SINGLE), /* SIO_EXT_WAKE# */
/* UART1_TXD */		PAD_NC(GPP_C13, NONE),
/* UART1_RTS# */	PAD_CFG_GPI(GPP_C14, NONE, DEEP), /* LCD_CBL_DET# */
/* UART1_CTS# */	PAD_NC(GPP_C15, NONE),
/* I2C0_SDA */		PAD_CFG_NF(GPP_C16, NONE, DEEP, NF1), /* TS_I2C_SDA */
/* I2C0_SCL */		PAD_CFG_NF(GPP_C17, NONE, DEEP, NF1), /* TS_I2C_SCL */
/* I2C1_SDA */		PAD_CFG_NF(GPP_C18, NONE, DEEP, NF1), /* I2C1_SDA_TP */
/* I2C1_SCL */		PAD_CFG_NF(GPP_C19, NONE, DEEP, NF1), /* I2C1_SCK_TP */
/* UART2_RXD */		PAD_CFG_NF(GPP_C20, NONE, DEEP, NF1), /* SERVOTX_UART */
/* UART2_TXD */		PAD_CFG_NF(GPP_C21, NONE, DEEP, NF1), /* SERVORX_UART */
/* UART2_RTS# */	PAD_NC(GPP_C22, NONE),
/* UART2_CTS# */	PAD_CFG_GPI_APIC(GPP_C23, NONE, PLTRST,
				 LEVEL, INVERT), /* TS_INT# */

/* SPI1_CS# */		PAD_CFG_GPI_APIC(GPP_D0, NONE, PLTRST,
				 EDGE_SINGLE, INVERT), /* MEDIACARD_IRQ# */
/* SPI1_CLK */		PAD_NC(GPP_D1, NONE),
/* SPI1_MISO */		PAD_NC(GPP_D2, NONE),
/* SPI1_MOSI */		PAD_CFG_GPI(GPP_D3, NONE, DEEP), /* RTC_DET# */
/* FASHTRIG */		PAD_NC(GPP_D4, NONE),
/* ISH_I2C0_SDA */	PAD_NC(GPP_D5, NONE),
/* ISH_I2C0_SCL */	PAD_NC(GPP_D6, NONE),
/* ISH_I2C1_SDA */	PAD_NC(GPP_D7, NONE),
/* ISH_I2C1_SCL */	PAD_NC(GPP_D8, NONE),
/* ISH_SPI_CS# */	PAD_CFG_GPI(GPP_D9, NONE, DEEP), /* IR_CAM_DET# */
/* ISH_SPI_CLK */	PAD_NC(GPP_D10, NONE),
/* ISH_SPI_MISO */	PAD_CFG_GPI(GPP_D11, NONE, DEEP), /* TBT_DET# */
/* ISH_SPI_MOSI */	PAD_NC(GPP_D12, NONE),
/* ISH_UART0_RXD */	PAD_NC(GPP_D13, NONE),
/* ISH_UART0_TXD */	PAD_NC(GPP_D14, NONE),
/* ISH_UART0_RTS# */	PAD_CFG_GPO(GPP_D15, 1, DEEP), /* WWAN_FULL_PWR_EN */
/* ISH_UART0_CTS# */	PAD_NC(GPP_D16, NONE),
/* DMIC_CLK1 */		PAD_CFG_GPI(GPP_D17, NONE, DEEP), /* KB_DET# */
/* DMIC_DATA1 */	PAD_CFG_GPI_APIC(GPP_D18, NONE, PLTRST,
				 EDGE_SINGLE, INVERT), /* H1_PCH_INT_ODL */
/* DMIC_CLK0 */		PAD_NC(GPP_D19, NONE),
/* DMIC_DATA0 */	PAD_NC(GPP_D20, NONE),
/* SPI1_IO2 */		PAD_CFG_GPO(GPP_D21, 1, DEEP), /* WWAN_BB_RST# */
/* SPI1_IO3 */		PAD_CFG_GPO(GPP_D22, 0, DEEP), /* WWAN_GPIO_PERST# */
/* I2S_MCLK */		PAD_CFG_GPI_SCI_LOW(GPP_D23, NONE, DEEP,
				 EDGE_SINGLE), /* WWAN_GPIO_WAKE# */

/* SATAXPCIE0 */	PAD_CFG_NF(GPP_E0, NONE, DEEP, NF1), /* HDD_DET# */
			/* M3042_PCIE#_SATA */
/* SATAXPCIE1 */	PAD_CFG_NF(GPP_E1, NONE, DEEP, NF1),
			/* M2880_PCIE_SATA# */
/* SATAXPCIE2 */	PAD_CFG_NF(GPP_E2, NONE, DEEP, NF1),
/* CPU_GP0 */		PAD_CFG_GPI(GPP_E3, NONE, DEEP), /* MEM_INTERLEAVED */
/* SATA_DEVSLP0 */	PAD_CFG_NF(GPP_E4, NONE, DEEP, NF1), /* HDD_DEVSLP */
/* SATA_DEVSLP1 */	PAD_CFG_NF(GPP_E5, NONE, DEEP, NF1), /* M3042_DEVSLP */
/* SATA_DEVSLP2 */	PAD_CFG_NF(GPP_E6, NONE, DEEP, NF1), /* M2280_DEVSLP */
/* CPU_GP1 */		PAD_CFG_GPO(GPP_E7, 1, DEEP), /* TOUCH_SCREEN_PD# */
/* SATALED# */		PAD_CFG_GPI(GPP_E8, NONE, DEEP), /* RECOVERY# */
/* USB2_OCO# */		PAD_CFG_NF(GPP_E9, NONE, DEEP, NF1), /* USB_OC0# */
/* USB2_OC1# */		PAD_CFG_NF(GPP_E10, NONE, DEEP, NF1), /* USB_OC1# */
/* USB2_OC2# */		PAD_CFG_NF(GPP_E11, NONE, DEEP, NF1), /* USB_OC2# */
/* USB2_OC3# */		PAD_NC(GPP_E12, NONE),
/* DDPB_HPD0 */		PAD_CFG_NF(GPP_E13, NONE, DEEP, NF1), /* HDMI_DP1_HPD */
/* DDPC_HPD1 */		PAD_CFG_NF(GPP_E14, NONE, DEEP, NF1), /* CPU_DP2_HPD */
/* DDPD_HPD2 */		PAD_CFG_GPI(GPP_E15, NONE, DEEP), /* H1_FLASH_WP */
/* DDPE_HPD3 */		PAD_NC(GPP_E16, NONE), /* FFS_INT2 (nostuff) */
/* EDP_HPD */		PAD_CFG_NF(GPP_E17, NONE, DEEP, NF1),
/* DDPB_CTRLCLK */	PAD_CFG_NF(GPP_E18, NONE, DEEP, NF1),
/* DDPB_CTRLDATA */	PAD_CFG_NF(GPP_E19, NONE, DEEP, NF1),
/* DDPC_CTRLCLK */	PAD_NC(GPP_E20, NONE),
/* DDPC_CTRLDATA */	PAD_NC(GPP_E21, NONE),
/* DDPD_CTRLCLK */	PAD_NC(GPP_E22, NONE),
/* DDPD_CTRLDATA */	PAD_NC(GPP_E23, NONE),

/* CNV_PA_BLANKING */	PAD_CFG_NF(GPP_F0, NONE, DEEP, NF1), /* CNV_COEX3 */
/* GPP_F1 */		PAD_NC(GPP_F1, NONE), /* T406 */
/* GPP_F2 */		PAD_NC(GPP_F2, NONE), /* T407 */
/* GPP_F3 */		PAD_NC(GPP_F3, NONE),
/* CNV_BRI_DT */	PAD_CFG_NF(GPP_F4, NONE, DEEP, NF1),
/* CNV_BRI_RSP */	PAD_CFG_NF(GPP_F5, NONE, DEEP, NF1),
/* CNV_RGI_DT */	PAD_CFG_NF(GPP_F6, NONE, DEEP, NF1),
/* CNV_RGI_RSP */	PAD_CFG_NF(GPP_F7, NONE, DEEP, NF1),
/* CNV_MFUART2_RXD */	PAD_CFG_NF(GPP_F8, NONE, DEEP, NF1), /* CNV_COEX2 */
/* CNV_MFUART2_TXD */	PAD_CFG_NF(GPP_F9, NONE, DEEP, NF1), /* CNV_COEX1 */
/* GPP_F10 */		PAD_NC(GPP_F10, NONE),
/* EMMC_CMD */		PAD_NC(GPP_F11, NONE),
/* EMMC_DATA0 */	PAD_NC(GPP_F12, NONE),
/* EMMC_DATA1 */	PAD_NC(GPP_F13, NONE),
/* EMMC_DATA2 */	PAD_NC(GPP_F14, NONE),
/* EMMC_DATA3 */	PAD_NC(GPP_F15, NONE),
/* EMMC_DATA4 */	PAD_NC(GPP_F16, NONE),
/* EMMC_DATA5 */	PAD_NC(GPP_F17, NONE),
/* EMMC_DATA6 */	PAD_NC(GPP_F18, NONE),
/* EMMC_DATA7 */	PAD_NC(GPP_F19, NONE),
/* EMMC_RCLK */		PAD_NC(GPP_F20, NONE),
/* EMMC_CLK */		PAD_NC(GPP_F21, NONE),
/* EMMC_RESET# */	PAD_NC(GPP_F22, NONE),
/* A4WP_PRESENT */	PAD_NC(GPP_F23, NONE),

/* SD_CMD */		PAD_CFG_GPI(GPP_G0, NONE, DEEP), /* CAM_MIC_CBL_DET# */
/* SD_DATA0 */		PAD_NC(GPP_G1, NONE),
/* SD_DATA1 */		PAD_NC(GPP_G2, NONE),
/* SD_DATA2 */		PAD_NC(GPP_G3, NONE), /* T383 */
/* SD_DATA3 */		PAD_CFG_GPI(GPP_G4, NONE, DEEP), /* CTLESS_DET# */
/* SD_CD# */		PAD_CFG_GPO(GPP_G5, 1, DEEP), /* HOST_SD_WP# */
/* SD_CLK */		PAD_CFG_GPO(GPP_G6, 1, DEEP), /* AUD_PWR_EN */
/* SD_WP */		PAD_NC(GPP_G7, NONE), /* T384 */

/* I2S2_SCLK */		PAD_NC(GPP_H0, NONE),
/* I2S2_SFRM */		PAD_CFG_NF(GPP_H1, NONE, DEEP, NF3), /* CNV_RF_RESET# */
/* I2S2_TXD */		PAD_CFG_NF(GPP_H2, NONE, DEEP, NF3), /* CLKREQ_CNV# */
/* I2S2_RXD */		PAD_CFG_GPO(GPP_H3, 0, DEEP), /* CNVI_EN# */
/* I2C2_SDA */		PAD_NC(GPP_H4, NONE), /* T388 */
/* I2C2_SCL */		PAD_NC(GPP_H5, NONE), /* T389 */
/* I2C3_SDA */		PAD_NC(GPP_H6, NONE), /* T378 */
/* I2C3_SCL */		PAD_NC(GPP_H7, NONE), /* T379 */
/* I2C4_SDA */		PAD_CFG_NF(GPP_H8, NONE, DEEP, NF1), /* I2C_SDA_H1 */
/* I2C4_SCL */		PAD_CFG_NF(GPP_H9, NONE, DEEP, NF1), /* I2C_SCL_H1 */
/* I2C5_SDA */		PAD_NC(GPP_H10, NONE),
/* I2C5_SCL */		PAD_NC(GPP_H11, NONE),
/* M2_SKT2_CFG2 */	PAD_NC(GPP_H14, NONE),
/* M2_SKT2_CFG3 */      PAD_CFG_GPO(GPP_H15, 1, DEEP), /* BT_RADIO_DIS# */
/* DDPF_CTRLCLK */	PAD_NC(GPP_H16, NONE),
/* DPPF_CTRLDATA */	PAD_NC(GPP_H17, NONE),
/* CPU_C10_GATE# */	PAD_CFG_NF(GPP_H18, NONE, DEEP, NF1), /* C10_GATE# */
/* TIMESYNC0 */		PAD_NC(GPP_H19, NONE),
/* IMGCLKOUT1 */	PAD_NC(GPP_H20, NONE),
/* GPP_H21 */		PAD_NC(GPP_H21, NONE),
/* GPP_H22 */		PAD_NC(GPP_H22, NONE),
/* GPP_H23 */		PAD_NC(GPP_H23, NONE),

/* BATLOW# */		PAD_CFG_NF(GPD0, NONE, DEEP, NF1), /* BATLOW# */
/* ACPRESENT */		PAD_CFG_NF(GPD1, NONE, DEEP, NF1), /* AC_PRESENT */
/* LAN_WAKE# */		PAD_CFG_NF(GPD2, NONE, DEEP, NF1), /* LAN_WAKE# */
/* SLP_S3# */		PAD_CFG_NF(GPD4, NONE, DEEP, NF1), /* SIO_SLP_S3# */
/* SLP_S4# */		PAD_CFG_NF(GPD5, NONE, DEEP, NF1), /* SIO_SLP_S4# */
/* SLP_A# */		PAD_CFG_NF(GPD6, NONE, DEEP, NF1), /* SIO_SLP_A# */
/* GPD7 */		PAD_NC(GPD7, NONE),
/* SUSCLK */		PAD_CFG_NF(GPD8, NONE, DEEP, NF1), /* SUSCLK */
/* SLP_WLAN# */		PAD_CFG_NF(GPD9, NONE, DEEP, NF1), /* SIO_SLP_WLAN# */
/* SLP_S5# */		PAD_CFG_NF(GPD10, NONE, DEEP, NF1), /* SIO_SLP_S5# */
/* LANPHYC */		PAD_CFG_NF(GPD11, NONE, DEEP, NF1), /* PM_LANPHY_EN */
};

/* Early pad configuration in bootblock */
static const struct pad_config early_gpio_table[] = {
/* M2_SKT2_CFG1 */	PAD_CFG_GPO(GPP_H13, 1, DEEP), /* M.2 SSD D3 cold */
/* SUSWARN# */		PAD_CFG_GPO(GPP_A13, 0, DEEP), /* Card reader D3 cold */
/* SUSACK# */		PAD_CFG_GPO(GPP_A15, 0, DEEP), /* Card reader D3 cold */
/* UART2_RXD */		PAD_CFG_NF(GPP_C20, NONE, DEEP, NF1), /* SERVOTX_UART */
/* UART2_TXD */		PAD_CFG_NF(GPP_C21, NONE, DEEP, NF1), /* SERVORX_UART */
/* SSD RESET pin will stay low first */
/* M2_SKT2_CFG0 */	PAD_CFG_GPO(GPP_H12, 0, DEEP), /* D3 cold RST */
/* I2C4_SDA */		PAD_CFG_NF(GPP_H8, NONE, DEEP, NF1), /* I2C_SDA_H1 */
/* I2C4_SCL */		PAD_CFG_NF(GPP_H9, NONE, DEEP, NF1), /* I2C_SCL_H1 */
/* DMIC_DATA1 */	PAD_CFG_GPI_APIC(GPP_D18, NONE, PLTRST,
				 EDGE_SINGLE, INVERT), /* H1_PCH_INT_ODL */
/* RESET# need to stay low before FULL_CARD_POWER_OFF assert */
/* SPI1_IO2 */		PAD_CFG_GPO(GPP_D21, 0, DEEP), /* WWAN_BB_RST# */
/* CPU_GP0 */		PAD_CFG_GPI(GPP_E3, NONE, DEEP), /* MEM_INTERLEAVED */
/* SATALED# */		PAD_CFG_GPI(GPP_E8, NONE, DEEP), /* RECOVERY# */
/* DDPD_HPD2 */		PAD_CFG_GPI(GPP_E15, NONE, DEEP), /* H1_FLASH_WP */
/* PWRBTN# */		PAD_CFG_NF(GPD3, UP_20K, DEEP, NF1), /* SIO_PWRBTN# */
/* M2_SKT2_CFG0 */	PAD_CFG_GPO(GPP_H12, 1, DEEP), /* D3 cold RST */
};

static const struct pad_config romstage_gpio_table[] = {
	/* Enable touchscreen, hold in reset */
	PAD_CFG_GPO(GPP_B21, 1, DEEP), /* PCH_3.3V_TS_EN */
	PAD_CFG_GPO(GPP_E7, 0, DEEP), /* TOUCH_SCREEN_PD# */
};

const struct pad_config *variant_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(gpio_table);
	return gpio_table;
}

const struct pad_config *variant_early_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(early_gpio_table);
	return early_gpio_table;
}

const struct pad_config *variant_romstage_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(romstage_gpio_table);
	return romstage_gpio_table;
}

static const struct cros_gpio cros_gpios[] = {
	CROS_GPIO_REC_AL(GPP_E8, CROS_GPIO_DEVICE_NAME),
	CROS_GPIO_WP_AH(GPP_E15, CROS_GPIO_DEVICE_NAME),
};

DECLARE_CROS_GPIOS(cros_gpios);
