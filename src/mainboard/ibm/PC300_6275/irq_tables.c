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

 #ifdef GETPIR			/* TODO: Drop this when copying to coreboot. */
 #include "pirq_routing.h"	/* TODO: Drop this when copying to coreboot. */
 #else				/* TODO: Drop this when copying to coreboot. */
 #include <arch/pirq_routing.h>
 #endif				/* TODO: Drop this when copying to coreboot. */

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
		 {0x00, (0x00 << 3) | 0x0, {{0x00, 0x0000}, {0x00, 0x0000}, {0x00, 0x0000}, {0x00, 0x0000}}, 0x0, 0x0},
		 {0x00, (0x01 << 3) | 0x0, {{0x00, 0x0000}, {0x00, 0x0000}, {0x00, 0x0000}, {0x00, 0x0000}}, 0x0, 0x0},
		 {0x00, (0x02 << 3) | 0x0, {{0x60, 0xdef8}, {0x61, 0xdef8}, {0x62, 0xdef8}, {0x63, 0xdef8}}, 0x0, 0x0},
		 {0x00, (0x03 << 3) | 0x0, {{0x63, 0xdef8}, {0x00, 0x0000}, {0x00, 0x0000}, {0x00, 0x0000}}, 0x0, 0x0},
		 {0x01, (0x01 << 3) | 0x0, {{0x61, 0xdef8}, {0x00, 0x0000}, {0x00, 0x0000}, {0x00, 0x0000}}, 0x0, 0x0},
		 {0x00, (0x14 << 3) | 0x0, {{0x60, 0xdef8}, {0x61, 0xdef8}, {0x62, 0xdef8}, {0x63, 0xdef8}}, 0x1, 0x0},
		 {0x00, (0x12 << 3) | 0x0, {{0x61, 0xdef8}, {0x62, 0xdef8}, {0x63, 0xdef8}, {0x60, 0xdef8}}, 0x2, 0x0},
		 {0x00, (0x10 << 3) | 0x0, {{0x62, 0xdef8}, {0x63, 0xdef8}, {0x60, 0xdef8}, {0x61, 0xdef8}}, 0x3, 0x0},
		 {0x01, (0x00 << 3) | 0x0, {{0x60, 0xdef8}, {0x61, 0xdef8}, {0x00, 0x0000}, {0x00, 0x0000}}, 0x6, 0x0},
	 }
 };

 unsigned long write_pirq_routing_table(unsigned long addr)
 {
	 return copy_pirq_routing_table(addr, &intel_irq_routing_table);
 }
