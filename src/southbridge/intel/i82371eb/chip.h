/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef SOUTHBRIDGE_INTEL_I82371EB_CHIP_H
#define SOUTHBRIDGE_INTEL_I82371EB_CHIP_H

#include <device/device.h>
#include <types.h>

struct southbridge_intel_i82371eb_config {
	bool ide0_enable;
	bool ide0_drive0_udma33_enable;
	bool ide0_drive1_udma33_enable;
	bool ide1_enable;
	bool ide1_drive0_udma33_enable;
	bool ide1_drive1_udma33_enable;
	bool ide_legacy_enable;
	bool usb_enable;
	bool gpi7_enable;  /* Select GPI7 (1) or SERIRQ (0) */
	bool gpo1516_enable; /* GPO15/GPO16 (1) vs. SUSB#/SUSC# (0) */
	bool gpo17_enable; /* GPO17 (1) vs. CPU_STP# (0) */
	bool gpo18_enable; /* GPO18 (1) vs. PCI_STP# (0) */
	bool gpo19_enable; /* GPO19 (1) vs. ZZ (0) */
	bool gpo20_enable; /* GPO20 (1) vs. SUS_STAT1# (0) */
	bool gpo21_enable; /* GPO21 (1) vs. SUS_STAT2# (0) */
	bool gpo2223_enable; /* GPO22/GPO23 (1) vs. XDIR#/XOE# (0) */
	bool gpo24_enable; /* GPO24 (1) vs. RTCCS# (0) */
	bool gpo25_enable; /* GPO25 (1) vs. RTCALE (0) */
	bool gpo26_enable; /* GPO26 (1) vs. KBCCS# (0) */
	bool reqa_gnta_enable;  /* PCI REQA and GNTA (1) vs GPI2, GPO9 (0) */
	bool reqb_gntb_enable;  /* PCI REQB and GNTB (1) vs GPI3, GPO10 (0) */
	bool reqc_gntc_enable;  /* PCI REQC and GNTC (1) vs GPI4, GPO11 (0) */
	bool positive_decode_enable; /* Enable positive decode on the PCI bus for forwarding to ISA*/
	bool pnp_decode_enable; /* Enable PnP address positive decode */

	/* acpi */
	u32 gpo; /* gpio output default */
	u8 lid_polarity;
	u8 thrm_polarity;
};

#endif /* SOUTHBRIDGE_INTEL_I82371EB_CHIP_H */
