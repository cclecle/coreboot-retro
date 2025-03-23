/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <stdint.h>
#include <console/console.h>
#include <device/device.h>
#include <device/pci.h>
#include <device/pci_ops.h>
#include <device/pci_ids.h>
#include <pc80/isa-dma.h>
#include <pc80/mc146818rtc.h>
#include <arch/ioapic.h>
#if CONFIG(HAVE_ACPI_TABLES)
#include <acpi/acpi.h>
#include <acpi/acpigen.h>
#endif
#include "i82371eb.h"
#include "chip.h"

static void isa_init(struct device *dev)
{
	u32 reg32;
	struct southbridge_intel_i82371eb_config *sb = dev->chip_info;

	/* Initialize the real time clock (RTC). */
	cmos_init(0);

	/*
	 * Enable special cycles, needed for soft poweroff.
	 */
	pci_or_config16(dev, PCI_COMMAND, PCI_COMMAND_SPECIAL);

	/*
	 * The PIIX4 can support the full ISA bus, or the Extended I/O (EIO)
	 * bus, which is a subset of ISA. We select the full ISA bus here.
	 */
	reg32 = pci_read_config32(dev, GENCFG);
	reg32 = ONOFF(1, 				reg32, ISA);		/* Select ISA */
	reg32 = ONOFF(sb->positive_decode_enable, 	reg32, POSITIVE_DECODE);/* Positive or Substractive Decode */
	reg32 = ONOFF(sb->pnp_decode_enable, 		reg32, PNP_DECODE);	/* Enable PnP address positive decode */
	reg32 = ONOFF(sb->gpi7_enable, 			reg32, GPI7SERIRQ);	/* Select GPI7 input or Serial IRQ function*/
	reg32 = ONOFF(sb->gpo1516_enable, 		reg32, GPO1516);	/* SUSB/SUSC or GPO1516 functionality*/
	reg32 = ONOFF(sb->gpo17_enable, 		reg32, GPO17);		/* SUSB/SUSC or GPO1516 functionality*/
	reg32 = ONOFF(sb->gpo18_enable, 		reg32, GPO18);		/* PCI_STP or GPO18 functionality*/
	reg32 = ONOFF(sb->gpo19_enable, 		reg32, GPO19);		/* ZZ or GPO19 functionality*/
	reg32 = ONOFF(sb->gpo20_enable, 		reg32, GPO20);		/* SUS_STAT1 or GPO20 functionality*/
	reg32 = ONOFF(sb->gpo21_enable, 		reg32, GPO21);		/* SUS_STAT2 or GPO21 functionality*/
	reg32 = ONOFF(sb->gpo2223_enable, 		reg32, GPO2223);	/* GPO22/23 functionality*/
	reg32 = ONOFF(sb->gpo24_enable, 		reg32, GPO24);		/* RTCCS or GPO24 functionality*/
	reg32 = ONOFF(sb->gpo25_enable, 		reg32, GPO25);		/* RTCALE or GPO25 functionality*/
	reg32 = ONOFF(sb->gpo26_enable, 		reg32, GPO26);		/* KBCCS or GPO26 */

	/* Enable REQ/GNT [A..C] signaling instead of GPI[2..4]/GPO[9..11] if configured*/
	reg32 = ONOFF(sb->reqa_gnta_enable, reg32, REQAGNTA);
	reg32 = ONOFF(sb->reqb_gntb_enable, reg32, REQBGNTB);
	reg32 = ONOFF(sb->reqc_gntc_enable, reg32, REQCGNTC);


	pci_write_config32(dev, GENCFG, reg32);

	/* Initialize ISA DMA. */
	isa_dma_init();

	/*
	 * Unlike most other southbridges the 82371EB doesn't have a built-in
	 * IOAPIC. Instead, 82371EB-based boards that support multiple CPUs
	 * have a discrete IOAPIC (Intel 82093AA) soldered onto the board.
	 *
	 * Thus, we can/must only enable the IOAPIC if it actually exists,
	 * i.e. the respective mainboard does "select IOAPIC".
	 */
	if (CONFIG(IOAPIC)) {
		u16 reg16;
		u8 ioapic_id = 2;

		/* Enable IOAPIC. */
		reg16 = pci_read_config16(dev, XBCS);
		reg16 |= (1 << 8); /* APIC Chip Select */
		pci_write_config16(dev, XBCS, reg16);

		/* Set and verify the IOAPIC ID. */
		setup_ioapic(VIO_APIC_VADDR, ioapic_id);
		if (ioapic_id != get_ioapic_id(VIO_APIC_VADDR))
			die("IOAPIC error!\n");
	}
}

#define ACPI_SCI_IRQ	9

void ioapic_get_sci_pin(u8 *gsi, u8 *irq, u8 *flags)
{
	*gsi = ACPI_SCI_IRQ;
	*irq = ACPI_SCI_IRQ;
	*flags = MP_IRQ_TRIGGER_LEVEL | MP_IRQ_POLARITY_HIGH;
}

static void sb_read_resources(struct device *dev)
{
	struct resource *res;

	pci_dev_read_resources(dev);

	res = new_resource(dev, 1);
	res->base = 0x0UL;
	res->size = 0x1000UL;
	res->limit = 0xffffUL;
	res->flags = IORESOURCE_IO | IORESOURCE_ASSIGNED | IORESOURCE_FIXED;

	res = new_resource(dev, 2);
	res->base = 0xff800000UL;
	res->size = 0x00800000UL; /* 8 MB for flash */
	res->flags = IORESOURCE_MEM | IORESOURCE_ASSIGNED | IORESOURCE_FIXED |
		IORESOURCE_RESERVE;

	res = new_resource(dev, 3); /* IOAPIC */
	res->base = IO_APIC_ADDR;
	res->size = 0x00001000;
	res->flags = IORESOURCE_MEM | IORESOURCE_ASSIGNED | IORESOURCE_FIXED |
		IORESOURCE_RESERVE;
}

static const struct device_operations isa_ops = {
	.read_resources		= sb_read_resources,
	.set_resources		= pci_dev_set_resources,
	.enable_resources	= pci_dev_enable_resources,
#if CONFIG(HAVE_ACPI_TABLES)
	.write_acpi_tables	= acpi_write_hpet,
	.acpi_fill_ssdt		= generate_cpu_entries,
#endif
	.init			= isa_init,
	.scan_bus		= scan_static_bus,
	.ops_pci		= 0, /* No subsystem IDs on 82371EB! */
};

static const struct pci_driver isa_driver __pci_driver = {
	.ops	= &isa_ops,
	.vendor	= PCI_VID_INTEL,
	.device	= PCI_DID_INTEL_82371AB_ISA,
};

static const struct pci_driver isa_SB_driver __pci_driver = {
	.ops	= &isa_ops,
	.vendor	= PCI_VID_INTEL,
	.device	= PCI_DID_INTEL_82371SB_ISA,
};
