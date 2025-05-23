/* SPDX-License-Identifier: GPL-2.0-only */

#include <acpi/acpi.h>
#include <arch/io.h>
#include <commonlib/bsd/bcd.h>
#include <console/console.h>
#include <fallback.h>
#include <pc80/mc146818rtc.h>
#include <rtc.h>
#include <security/vboot/vbnv.h>
#include <security/vboot/vbnv_layout.h>
#include <types.h>
#include <version.h>

static void cmos_reset_date(void)
{
	printk(BIOS_DEBUG, "cmos_reset_date()\n");
	/* Now setup a default date equals to the build date */
	struct rtc_time time = {
		.sec = 0,
		.min = 0,
		.hour = 1,
		.mday = bcd2bin(coreboot_build_date.day),
		.mon = bcd2bin(coreboot_build_date.month),
		.year = (bcd2bin(coreboot_build_date.century) * 100) +
			bcd2bin(coreboot_build_date.year),
		.wday = bcd2bin(coreboot_build_date.weekday)
	};
	rtc_set(&time);
	printk(BIOS_DEBUG, "END cmos_reset_date()\n");
}

int cmos_checksum_valid(int range_start, int range_end, int cks_loc)
{
	printk(BIOS_DEBUG, "cmos_checksum_valid()\n");
	int i;
	u16 sum, old_sum;

	if (CONFIG(STATIC_OPTION_TABLE))
		return 1;

	sum = 0;
	for (i = range_start; i <= range_end; i++)
		sum += cmos_read(i);
	old_sum = ((cmos_read(cks_loc) << 8) | cmos_read(cks_loc + 1)) &
		  0x0ffff;
	printk(BIOS_DEBUG, "END cmos_checksum_valid()\n");
	return sum == old_sum;
}

void cmos_set_checksum(int range_start, int range_end, int cks_loc)
{
	printk(BIOS_DEBUG, "cmos_set_checksum()\n");
	int i;
	u16 sum;

	sum = 0;
	for (i = range_start; i <= range_end; i++)
		sum += cmos_read(i);
	cmos_write(((sum >> 8) & 0x0ff), cks_loc);
	cmos_write(((sum >> 0) & 0x0ff), cks_loc + 1);
	printk(BIOS_DEBUG, "END cmos_set_checksum()\n");
}

/* See if the CMOS error condition has been flagged */
int cmos_error(void)
{
	printk(BIOS_DEBUG, "cmos_error()\n");
	return (cmos_read(RTC_VALID) & RTC_VRT) == 0;
}

#define RTC_CONTROL_DEFAULT (RTC_24H)
#define RTC_FREQ_SELECT_DEFAULT (RTC_REF_CLCK_32KHZ | RTC_RATE_1024HZ)

static bool __cmos_init(bool invalid)
{
	printk(BIOS_DEBUG, "__cmos_init()\n");
	bool cmos_invalid;
	bool checksum_invalid = false;
	bool cleared_cmos = false;
	size_t i;

	/*
	 * Avoid clearing pending interrupts and resetting the RTC control
	 * register in the resume path because the Linux kernel relies on
	 * this to know if it should restart the RTC timer queue if the wake
	 * was due to the RTC alarm.
	 */
	if (ENV_RAMSTAGE && acpi_is_wakeup_s3())
		return false;

	printk(BIOS_DEBUG, "RTC Init\n");

	/* See if there has been a CMOS power problem. */
	cmos_invalid = cmos_error();

	printk(BIOS_DEBUG, "?? 1\n");
	if (CONFIG(USE_OPTION_TABLE)) {
		printk(BIOS_DEBUG, "?? 2\n");
		/* See if there is a CMOS checksum error */
		checksum_invalid = !cmos_checksum_valid(PC_CKS_RANGE_START,
						PC_CKS_RANGE_END, PC_CKS_LOC);
	}
	printk(BIOS_DEBUG, "?? 3\n");

	if (cmos_invalid || invalid)
	{
		printk(BIOS_DEBUG, "?? 4\n");
		cmos_disable_rtc();
	}

	printk(BIOS_DEBUG, "?? 5\n");
	if (invalid || cmos_invalid || checksum_invalid) {
		printk(BIOS_DEBUG, "?? 6\n");
		if (!CONFIG(USE_OPTION_TABLE)) {
			printk(BIOS_DEBUG, "?? 7\n");
			cmos_write(0, 0x01);
			cmos_write(0, 0x03);
			cmos_write(0, 0x05);
			for (i = 10; i < 128; i++)
				cmos_write(0, i);
			cleared_cmos = true;
		}
		printk(BIOS_DEBUG, "?? 8\n");

		if (cmos_invalid || invalid)
		{
			printk(BIOS_DEBUG, "?? 9\n");
			cmos_reset_date();
		}

		printk(BIOS_WARNING, "RTC:%s%s%s%s\n",
			invalid ? " Clear requested":"",
			cmos_invalid ? " Power Problem":"",
			checksum_invalid ? " Checksum invalid":"",
			cleared_cmos ? " zeroing cmos":"");
	}
	printk(BIOS_DEBUG, "?? 10\n");

	/* Setup the real time clock */
	cmos_write(RTC_CONTROL_DEFAULT, RTC_CONTROL);
	printk(BIOS_DEBUG, "?? 11\n");
	/* Setup the frequency it operates at */
	cmos_write(RTC_FREQ_SELECT_DEFAULT, RTC_FREQ_SELECT);
	printk(BIOS_DEBUG, "?? 12\n");
	/* Ensure all reserved bits are 0 in register D */
	cmos_write(RTC_VRT, RTC_VALID);
	printk(BIOS_DEBUG, "?? 13\n");

	if (CONFIG(USE_OPTION_TABLE)) {
		printk(BIOS_DEBUG, "?? 14\n");
		/* See if there is a LB CMOS checksum error */
		checksum_invalid = !cmos_lb_cks_valid();
		printk(BIOS_DEBUG, "?? 15\n");
		if (checksum_invalid)
			printk(BIOS_DEBUG, "RTC: coreboot checksum invalid\n");

		printk(BIOS_DEBUG, "?? 16\n");
		/* Make certain we have a valid checksum */
		cmos_set_checksum(PC_CKS_RANGE_START, PC_CKS_RANGE_END, PC_CKS_LOC);
		printk(BIOS_DEBUG, "?? 17\n");
	}

	printk(BIOS_DEBUG, "?? 18\n");
	/* Clear any pending interrupts */
	cmos_read(RTC_INTR_FLAGS);

	printk(BIOS_DEBUG, "END __cmos_init()\n");
	return cleared_cmos;
}

static void cmos_init_vbnv(bool invalid)
{
	printk(BIOS_DEBUG, "cmos_init_vbnv()\n");
	uint8_t vbnv[VBOOT_VBNV_BLOCK_SIZE];

	/* __cmos_init() will clear vbnv contents when a known rtc failure
	   occurred with !CONFIG(USE_OPTION_TABLE). However, __cmos_init() may
	   clear vbnv data for other internal reasons. For that, always back up
	   the vbnv contents and conditionally save them when __cmos_init()
	   indicates CMOS was cleared. */
	read_vbnv_cmos(vbnv);

	if (__cmos_init(invalid))
		save_vbnv_cmos(vbnv);
	printk(BIOS_DEBUG, "END cmos_init_vbnv()\n");
}

void cmos_init(bool invalid)
{
	printk(BIOS_DEBUG, "cmos_init()\n");
	if (ENV_SMM)
		return;

	if (CONFIG(VBOOT_VBNV_CMOS))
		cmos_init_vbnv(invalid);
	else
		__cmos_init(invalid);
	printk(BIOS_DEBUG, "END cmos_init()\n");
}

/*
 * Upon return the caller is guaranteed 244 microseconds to complete any
 * RTC operations. wait_uip may be called a single time prior to multiple
 * accesses, but sequences requiring more time should call wait_uip again.
 */
static void wait_uip(void)
{
	printk(BIOS_DEBUG, "wait_uip()\n");
	while (cmos_read(RTC_REG_A) & RTC_UIP)
		;
	printk(BIOS_DEBUG, "END wait_uip()\n");
}

/* Perform a sanity check of current date and time. */
static int cmos_date_invalid(void)
{
	printk(BIOS_DEBUG, "cmos_date_invalid()\n");
	struct rtc_time now;

	rtc_get(&now);
	printk(BIOS_DEBUG, "END cmos_date_invalid()\n");
	return rtc_invalid(&now);
}

/*
 * If the CMOS is cleared, the rtc_reg has the invalid date. That
 * hurts some OSes. Even if we don't set USE_OPTION_TABLE, we need
 * to make sure the date is valid.
 */
void cmos_check_update_date(void)
{
	printk(BIOS_DEBUG, "cmos_check_update_date()\n");
	u8 year, century = 0;

	wait_uip();
	if (CONFIG(USE_PC_CMOS_ALTCENTURY))
		century = cmos_read(RTC_CLK_ALTCENTURY);
	year = cmos_read(RTC_CLK_YEAR);

	/*
	 * If century is 0xFF, 100% that the CMOS is cleared.
	 * In addition, check the sanity of all values and reset the date in case of
	 * insane values.
	 */
	if (century > 0x99 || year > 0x99 || cmos_date_invalid()) /* Invalid date */
		cmos_reset_date();
	printk(BIOS_DEBUG, "END cmos_check_update_date()\n");
}

int rtc_set(const struct rtc_time *time)
{
	printk(BIOS_DEBUG, "rtc_set()\n");
	cmos_write(bin2bcd(time->sec), RTC_CLK_SECOND);
	cmos_write(bin2bcd(time->min), RTC_CLK_MINUTE);
	cmos_write(bin2bcd(time->hour), RTC_CLK_HOUR);
	cmos_write(bin2bcd(time->mday), RTC_CLK_DAYOFMONTH);
	cmos_write(bin2bcd(time->mon), RTC_CLK_MONTH);
	cmos_write(bin2bcd(time->year % 100), RTC_CLK_YEAR);
	if (CONFIG(USE_PC_CMOS_ALTCENTURY))
		cmos_write(bin2bcd(time->year / 100), RTC_CLK_ALTCENTURY);
	cmos_write(bin2bcd(time->wday + 1), RTC_CLK_DAYOFWEEK);
	printk(BIOS_DEBUG, "END rtc_set()\n");
	return 0;
}

int rtc_get(struct rtc_time *time)
{
	printk(BIOS_DEBUG, "rtc_get()\n");
	wait_uip();
	time->sec = bcd2bin(cmos_read(RTC_CLK_SECOND));
	time->min = bcd2bin(cmos_read(RTC_CLK_MINUTE));
	time->hour = bcd2bin(cmos_read(RTC_CLK_HOUR));
	time->mday = bcd2bin(cmos_read(RTC_CLK_DAYOFMONTH));
	time->mon = bcd2bin(cmos_read(RTC_CLK_MONTH));
	time->year = bcd2bin(cmos_read(RTC_CLK_YEAR));
	if (CONFIG(USE_PC_CMOS_ALTCENTURY)) {
		time->year += bcd2bin(cmos_read(RTC_CLK_ALTCENTURY)) * 100;
	} else {
		time->year += 1900;
		if (time->year < 1970)
			time->year += 100;
	}
	time->wday = bcd2bin(cmos_read(RTC_CLK_DAYOFWEEK)) - 1;
	printk(BIOS_DEBUG, "END rtc_get()\n");
	return 0;
}

/*
 * Signal coreboot proper completed -- just before running payload
 * or jumping to ACPI S3 wakeup vector.
 */
void set_boot_successful(void)
{
	printk(BIOS_DEBUG, "set_boot_successful()\n");
	uint8_t index, byte;

	index = inb(RTC_PORT_BANK0(0)) & 0x80;
	index |= RTC_BOOT_BYTE;
	outb(index, RTC_PORT_BANK0(0));

	byte = inb(RTC_PORT_BANK0(1));

	if (CONFIG(SKIP_MAX_REBOOT_CNT_CLEAR)) {
		/*
		 * Set the fallback boot bit to allow for recovery if
		 * the payload fails to boot.
		 * It is the responsibility of the payload to reset
		 * the normal boot bit to 1 if desired
		 */
		byte &= ~RTC_BOOT_NORMAL;
	} else {
		/* If we are in normal mode set the boot count to 0 */
		if (byte & RTC_BOOT_NORMAL)
			byte &= 0x0f;
	}

	outb(byte, RTC_PORT_BANK0(1));
	printk(BIOS_DEBUG, "END set_boot_successful()\n");
}
