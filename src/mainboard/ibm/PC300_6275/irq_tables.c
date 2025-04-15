/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 200x TODO <TODO@TODO>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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

#include <arch/pirq_routing.h>
#include <console/console.h>
#include <device/pci_def.h>
#include <string.h>

 const struct irq_routing_table intel_irq_routing_table = {
	 PIRQ_SIGNATURE,		/* u32 signature */
	 PIRQ_VERSION,		/* u16 version */
	 32 + 16 * CONFIG_IRQ_SLOT_COUNT,		/* Max. number of devices on the bus */
	 0x00,			/* Interrupt router bus */
	 (0x02 << 3) | 0x0,	/* Interrupt router dev */
	 0,			/* IRQs devoted exclusively to PCI usage */
	 0x8086,			/* Vendor */
	 0x122e,			/* Device */
	 0,			/* Miniport */
	 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, /* u8 rfu[11] */
	 0x9f,			/* Checksum (has to be set to some value that
				  * would give 0 after the sum of all bytes
				  * for this structure (including checksum).
				  */
	 {
		 /* bus,        dev | fn,   {link, bitmap}, {link, bitmap}, {link, bitmap}, {link, bitmap}, slot, rfu */

		 // Host bridge => DISABLED ?
		 //{0x00, (0x00 << 3) | 0x0, {{0x60, 0xdef8}, {0x61, 0xdef8}, {0x62, 0xdef8}, {0x63, 0xdef8}}, 0x0, 0x0},

		 // PCI/AGP bridge
		 {0x00, (0x01 << 3) | 0x0, {{0x60, 0xdef8}, {0x61, 0xdef8}, {0x62, 0xdef8}, {0x63, 0xdef8}}, 0x0, 0x0},

		 // ISA Bridge
		 {0x00, (0x02 << 3) | 0x0, {{0x60, 0xdef8}, {0x61, 0xdef8}, {0x62, 0xdef8}, {0x63, 0xdef8}}, 0x0, 0x0},

		 // ??  => DISABLED ?
		 //{0x00, (0x03 << 3) | 0x0, {{0x63, 0xdef8}, {0x00, 0x0000}, {0x00, 0x0000}, {0x00, 0x0000}}, 0x0, 0x0},

		 // Actual ports
		 {0x00, (0x0e << 3) | 0x0, {{0x60, 0xdef8}, {0x61, 0xdef8}, {0x62, 0xdef8}, {0x63, 0xdef8}}, 0x1, 0x0},
		 {0x00, (0x0c << 3) | 0x0, {{0x61, 0xdef8}, {0x62, 0xdef8}, {0x63, 0xdef8}, {0x60, 0xdef8}}, 0x2, 0x0},
		 {0x00, (0x0a << 3) | 0x0, {{0x62, 0xdef8}, {0x63, 0xdef8}, {0x60, 0xdef8}, {0x61, 0xdef8}}, 0x3, 0x0},

		 // ??  => DISABLED ?
		 //{0x01, (0x01 << 3) | 0x0, {{0x61, 0xdef8}, {0x00, 0x0000}, {0x00, 0x0000}, {0x00, 0x0000}}, 0x0, 0x0},
		 //{0x01, (0x00 << 3) | 0x0, {{0x60, 0xdef8}, {0x61, 0xdef8}, {0x00, 0x0000}, {0x00, 0x0000}}, 0x6, 0x0},
	 }
 };

/*
 unsigned long write_pirq_routing_table(unsigned long addr)
 {
	 return copy_pirq_routing_table(addr, &intel_irq_routing_table);
 }
*/

 static void write_pirq_info(struct irq_info *pirq_info, u8 bus, u8 devfn,
	u8 link0, u16 bitmap0, u8 link1, u16 bitmap1,
	u8 link2, u16 bitmap2, u8 link3, u16 bitmap3, u8 slot, u8 rfu)
{
pirq_info->bus = bus;
pirq_info->devfn = devfn;
pirq_info->irq[0].link = link0;
pirq_info->irq[0].bitmap = bitmap0;
pirq_info->irq[1].link = link1;
pirq_info->irq[1].bitmap = bitmap1;
pirq_info->irq[2].link = link2;
pirq_info->irq[2].bitmap = bitmap2;
pirq_info->irq[3].link = link3;
pirq_info->irq[3].bitmap = bitmap3;
pirq_info->slot = slot;
pirq_info->rfu = rfu;
}


unsigned long write_pirq_routing_table(unsigned long addr)
{
	struct irq_routing_table *pirq;
	struct irq_info *pirq_info;
	u32 slot_num;

	u8 sum = 0;
	int i;

	// Align the table to be 16 byte aligned.
	addr = ALIGN_UP(addr, 16);

	// This table must be between 0xf0000 & 0x100000
	printk(BIOS_INFO, "Writing IRQ routing tables to 0x%lx ...\n", addr);

	pirq = (void *)(addr);

	pirq->signature = PIRQ_SIGNATURE;
	pirq->version = PIRQ_VERSION;

	pirq->rtr_bus = 0;
	pirq->rtr_devfn = PCI_DEVFN(0x02, 0);

	pirq->exclusive_irqs = 0x0c80;

	pirq->rtr_vendor = 0x8086;
	pirq->rtr_device = 0x122e;

	pirq->miniport_data = 0;

	memset(pirq->rfu, 0, sizeof(pirq->rfu));

	pirq_info = (void *)(&pirq->slots);
	slot_num = 0;

	// pci slot 1
	//{0x00, (0x14 << 3) | 0x0, {{0x60, 0xdef8}, {0x61, 0xdef8}, {0x62, 0xdef8}, {0x63, 0xdef8}}, 0x1, 0x0},
	write_pirq_info(pirq_info, 0, PCI_DEVFN(0x14, 0),
			0x60, 0xdef8, 0x61, 0xdef8, 0x62, 0xdef8, 0x63, 0xdef8, 1, 0);
	pirq_info++;
	slot_num++;

	// pci slot 2
	//{0x00, (0x12 << 3) | 0x0, {{0x61, 0xdef8}, {0x62, 0xdef8}, {0x63, 0xdef8}, {0x60, 0xdef8}}, 0x2, 0x0},
	write_pirq_info(pirq_info, 0, PCI_DEVFN(0x12, 0),
			0x61, 0xdef8, 0x62, 0xdef8, 0x63, 0xdef8, 0x60, 0xdef8, 2, 0);
	pirq_info++;
	slot_num++;

	// pci slot 3
	//{0x00, (0x10 << 3) | 0x0, {{0x62, 0xdef8}, {0x63, 0xdef8}, {0x60, 0xdef8}, {0x61, 0xdef8}}, 0x3, 0x0},
	write_pirq_info(pirq_info, 0, PCI_DEVFN(0x10, 0),
			0x62, 0xdef8, 0x63, 0xdef8, 0x60, 0xdef8, 0x61, 0xdef8, 3, 0);
	pirq_info++;
	slot_num++;

	// 82371AB/EB/MB PIIX4/E/M EIDE Controller
	//{0x00, (0x02 << 3) | 0x0, {{0x60, 0xdef8}, {0x61, 0xdef8}, {0x62, 0xdef8}, {0x63, 0xdef8}}, 0x0, 0x0},
	write_pirq_info(pirq_info, 0, PCI_DEVFN(0x02, 1),
			0x60, 0xdef8, 0x61, 0xdef8, 0x62, 0xdef8, 0x63, 0xdef8, 0, 0);
	pirq_info++;
	slot_num++;

	// 82443BX/ZX 440BX/ZX PCI to AGP Bridge
	//{0x00, (0x01 << 3) | 0x0, {{0x00, 0x0000}, {0x00, 0x0000}, {0x00, 0x0000}, {0x00, 0x0000}}, 0x0, 0x0},
	write_pirq_info(pirq_info, 0, PCI_DEVFN(0x01, 0),
			0x60, 0xdef8, 0x61, 0xdef8, 0x62, 0xdef8, 0x63, 0xdef8, 0, 0);
	pirq_info++;
	slot_num++;

	pirq->size = 32 + 16 * slot_num;

	{
		const u8 *const v = (u8 *)(pirq);
		for (i = 0; i < pirq->size; i++)
			sum += v[i];
	}

	sum = pirq->checksum - sum;

	if (sum != pirq->checksum)
		pirq->checksum = sum;

	printk(BIOS_INFO, "%s DONE.\n", __func__);

	return (unsigned long)pirq_info;
}

