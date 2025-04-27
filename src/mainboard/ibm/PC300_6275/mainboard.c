/* SPDX-License-Identifier: GPL-2.0-only */

#include <device/pci_ops.h>
#include <southbridge/intel/i82371eb/i82371eb.h>
#include <console/console.h>

/**
 * Mainboard specific enables.
 *
 * @param chip_info Ignored
 */


static void mainboard_final(void *chip_info)
{
	u8 reg8;
	printk(BIOS_INFO, "300GL mainboard init\n");
	return;

	outb(0x55, 0x370); // enter SIO config mode

	outb(0x08, 0x370 + 0x07); // access to logical device 8 (Aux config)

	reg8 = inb(0x370 + 0xb4); // read SMI Enable Register 1
	printk(BIOS_INFO, "SMI Enable Register 1: 0x%x\n",reg8);

	reg8 = inb(0x370 + 0xb5); // read SMI Enable Register 2
	printk(BIOS_INFO, "SMI Enable Register 2: 0x%x\n",reg8);

	// Enable mouse & Kb SMI:
	reg8 |= (1 << 1) | (1 << 0);
	// Enable nSMI Interrupt onto Serial IRQ
	reg8 |= (1 << 6);
	outb(reg8, 0x370 + 0xb5); // read SMI Enable Register 2

	reg8 = inb(0x370 + 0xc0); // read Pin Multiplex Controls
	printk(BIOS_INFO, "Pin Multiplex Controls: 0x%x\n",reg8);

	outb(0xaa, 0x370); // exit SIO config mode

}



struct chip_operations mainboard_ops = {
	.final = mainboard_final
};
