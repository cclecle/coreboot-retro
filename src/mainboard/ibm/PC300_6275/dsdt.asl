/* SPDX-License-Identifier: GPL-2.0-only */

#include <southbridge/intel/i82371eb/i82371eb.h>
#include <acpi/acpi.h>


DefinitionBlock (
	"dsdt.aml",
	"DSDT",
	ACPI_DSDT_REV_2,
	OEM_ID,
	ACPI_TABLE_CREATOR,
	1
	) {

	#include <acpi/dsdt_top.asl>
	/* \_SB scope defining the main processor is generated in SSDT. */

	#include <arch/x86/acpi/post.asl>

	/*
	 * Intel 82371EB (PIIX4E) datasheet, section 7.2.3, page 142
	 *
	 * 0: soft off/suspend to disk					S5
	 * 1: suspend to ram						S3
	 * 2: powered on suspend, context lost				S2
	 *    Note: 'context lost' means the CPU restarts at the reset
	 *          vector
	 * 3: powered on suspend, CPU context lost			S1
	 *    Note: Looks like 'CPU context lost' does _not_ mean the
	 *          CPU restarts at the reset vector. Most likely only
	 *          caches are lost, so both 0x3 and 0x4 map to ACPI S1
	 * 4: powered on suspend, context maintained			S1
	 * 5: working (clock control)					S0
	 * 6: reserved
	 * 7: reserved
	 */
	Name (\_S0, Package () { 0x05, 0x05, 0x00, 0x00})
	Name (\_S1, Package () { 0x03, 0x03, 0x00, 0x00})
	Name (\_S5, Package () { 0x00, 0x00, 0x00, 0x00})

	OperationRegion (GPOB, SystemIO, DEFAULT_PMBASE + DEVCTL, 0x10)
	Field (GPOB, ByteAcc, NoLock, Preserve) {
		Offset (0x03),
		TO12,   1, /* Device trap 12 */
		Offset (0x08),
		FANM,   1, /* GPO0, meant for fan */
		Offset (0x09),
		PLED,   1, /* GPO8, meant for power LED. Per PIIX4 datasheet */
		    ,   3, /* this goes low when power is cut from its core. */
		    ,   2,
		    ,   16,
		MSG0,   1 /* GPO30, message LED */
	}

	/* Prepare To Sleep, Arg0 is target S-state */
	Method (\_PTS, 1, NotSerialized) {
		/* Disable fan, blink power LED, if not turning off */
		If (Arg0 != 0x05) {
		    FANM = 0
		    PLED = 0
		}

		/* Arms SMI for device 12 */
		TO12 = 1
		/* Put out a POST code */
		DBG0 = Arg0 | 0xF0
	}

	Method (\_WAK, 1, NotSerialized) {
		/* Re-enable fan, stop power led blinking */
		FANM = 1
		PLED = 1
		/* wake OK */
		Return(Package(0x02){0x00, 0x00})
	}

	/* Root of the bus hierarchy */
	Scope (\_SB) {
		#include <southbridge/intel/i82371eb/acpi/intx.asl>

		PCI_INTX_DEV(LNKA, \_SB.PCI0.PX40.PIRA, 1)
		PCI_INTX_DEV(LNKB, \_SB.PCI0.PX40.PIRB, 2)
		PCI_INTX_DEV(LNKC, \_SB.PCI0.PX40.PIRC, 3)
		PCI_INTX_DEV(LNKD, \_SB.PCI0.PX40.PIRD, 4)

		/* Top PCI device */
		Device (PCI0) {
			Name (_HID, EisaId ("PNP0A03"))
			Name (_UID, 0x00)
			Name (_BBN, 0x00)

			/* PCI Routing Table */
			Name (_PRT, Package () {
				/* PCI/AGP bridge*/
				Package (0x04) { 0x0001FFFF, 0, LNKA, 0 },
				Package (0x04) { 0x0001FFFF, 1, LNKB, 0 },
				Package (0x04) { 0x0001FFFF, 2, LNKC, 0 },
				Package (0x04) { 0x0001FFFF, 3, LNKD, 0 },

				/* ISA Bridge*/
				Package (0x04) { 0x0002FFFF, 0, LNKA, 0 },
				Package (0x04) { 0x0002FFFF, 1, LNKB, 0 },
				Package (0x04) { 0x0002FFFF, 2, LNKC, 0 },
				Package (0x04) { 0x0002FFFF, 3, LNKD, 0 },

				Package (0x04) { 0x0010FFFF, 0, LNKC, 0 },
				Package (0x04) { 0x0010FFFF, 1, LNKD, 0 },
				Package (0x04) { 0x0010FFFF, 2, LNKA, 0 },
				Package (0x04) { 0x0010FFFF, 3, LNKB, 0 },

				Package (0x04) { 0x0012FFFF, 0, LNKB, 0 },
				Package (0x04) { 0x0012FFFF, 1, LNKC, 0 },
				Package (0x04) { 0x0012FFFF, 2, LNKD, 0 },
				Package (0x04) { 0x0012FFFF, 3, LNKA, 0 },

				Package (0x04) { 0x0014FFFF, 0, LNKA, 0 },
				Package (0x04) { 0x0014FFFF, 1, LNKB, 0 },
				Package (0x04) { 0x0014FFFF, 2, LNKC, 0 },
				Package (0x04) { 0x0014FFFF, 3, LNKD, 0 },
			})
			#include <northbridge/intel/i440bx/acpi/sb_pci0_crs.asl>
			#include <southbridge/intel/i82371eb/acpi/isabridge.asl>
			#include <mainboard/ibm/PC300_6275/i82371eb.asl>

			Device(SIO) {
				Name (_HID, EisaId("PNP0A05"))
				Name (_UID, 0)

				// Floppy controller
				Device (FDC0) {
					Name (_HID, EISAID ("PNP0700"))
					Name (_STA, 0x0F)
					Name (_CRS, ResourceTemplate() {
						FixedIO (0x03F0, 0x06)
						IRQNoFlags () {6}
						DMA (Compatibility, NotBusMaster, Transfer8) {2}
					})
				}

				// Keyboard
				Device (PS2K) {
					Name (_HID, EISAID("PNP0303"))
					Name (_CID, EISAID("PNP030B"))
					Name (_STA, 0x0F)
					Name (_CRS, ResourceTemplate () {
							FixedIO (0x0060, 0x01)
							FixedIO (0x0064, 0x01)
							IRQNoFlags () {1}
					})

				}

				// Mouse
				Device (PS2M) {
					Name (_HID, EISAID("PNP0F13"))
					Name (_STA, 0x0F)
					Name (_CRS, ResourceTemplate () {
						IRQNoFlags () {12}
					})
				}
			}
		}
	}

	/* ACPI Message */
	Scope (\_SI) {
		Method (_MSG, 1, NotSerialized) {
			If (Arg0 == 0) {
				MSG0 = 1
			}
			Else {
				MSG0 = 0
			}
		}
	}
}
