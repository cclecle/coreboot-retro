/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <bootblock_common.h>
#include <superio/smsc/smscsuperio/smscsuperio.h>


#define SERIAL_DEV PNP_DEV(0x370, SMSCSUPERIO_SP1)

void bootblock_mainboard_early_init(void)
{
	smscsuperio_enable_serial(SERIAL_DEV, CONFIG_TTYS0_BASE);
}
