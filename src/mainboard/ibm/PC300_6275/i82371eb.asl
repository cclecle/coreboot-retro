/* SPDX-License-Identifier: GPL-2.0-only */

#include "southbridge/intel/i82371eb/i82371eb.h"

/* Declares assorted devices that fall under this southbridge. */

// Device 2, function 0 on bus 0 (PCI / ISA Bridge)
Device (PX40)
{
	Name(_ADR, 0x00020000)
	OperationRegion (PIRQ, PCI_Config, 0x60, 0x04)
	Field (PIRQ, ByteAcc, NoLock, Preserve)
	{
		PIRA,   8,
		PIRB,   8,
		PIRC,   8,
		PIRD,   8
	}
}

// Device 2, function 3 on bus 0 (Power Management)
Device (PX43)
{
	Name (_ADR, 0x00020003)
	Method (_CRS, 0, NotSerialized)
	{
		Name (BUF1, ResourceTemplate ()
		{
			/* PM register ports */
			IO (Decode16, PM_IO_BASE, PM_IO_BASE, 0x01, 0x40, )
			/* SMBus register ports */
			IO (Decode16, SMBUS_IO_BASE, SMBUS_IO_BASE, 0x01, 0x10, )
		})
		Return (BUF1)
	}
}

// Device 2, function 1 on bus 0 (IDE controller)
Device (PX41)
{
	Name (_ADR, 0x00020003)
	Method (_CRS, 0, NotSerialized)
	{
		Name (BUF1, ResourceTemplate ()
		{
			IO (Decode16, 0xfff0, 0xfff0, 0x01, 0x08, ) // Primary IDE channel
			IO (Decode16, 0xfff8, 0xfff8, 0x01, 0x08, ) // Secondary IDE channel
		})
		Return (BUF1)
	}
}


//OperationRegion (S1XX, SystemIO, 0xB2, 0x01) //0xB2 is GPIO... on PCI config space :-/ And IDK what in IO space
//Field (S1XX, ByteAcc, NoLock, Preserve)
//{
//	FXS1,   8
//}

/* PNP Motherboard Resources */
Device (SYSR)
{
	Name (_HID, EisaId ("PNP0C02"))
	Name (_UID, 0x02)
	Method (_CRS, 0, NotSerialized)
	{
		Name (BUF1, ResourceTemplate ()
		{
			/* PIIX4E ports */
			/* Aliased DMA ports */
			IO (Decode16, 0x0010, 0x0010, 0x01, 0x10, ) // OK
			/* Aliased PIC ports */
			IO (Decode16, 0x0022, 0x0022, 0x01, 0x1E, ) // OK but overlap on 2 byte after each blocks
			/* Aliased timer ports */
			IO (Decode16, 0x0050, 0x0050, 0x01, 0x04, ) // OK
			IO (Decode16, 0x0062, 0x0062, 0x01, 0x02, ) // Ok but overlap on 1 byte after each blocks
			IO (Decode16, 0x0065, 0x0065, 0x01, 0x0B, ) // also overlaps
			IO (Decode16, 0x0074, 0x0074, 0x01, 0x0C, ) // ok but missed some before
			IO (Decode16, 0x0091, 0x0091, 0x01, 0x03, ) // missed 90 ? 92 masked but not written in the doc, and why not masking 94+ ?
			IO (Decode16, 0x00A2, 0x00A2, 0x01, 0x1E, ) // This probably mask APM Control and Status
			//IO (Decode16, 0x00E0, 0x00E0, 0x01, 0x10, ) // ??
			//IO (Decode16, 0x0294, 0x0294, 0x01, 0x04, ) // ??
			IO (Decode16, 0x04D0, 0x04D0, 0x01, 0x02, ) // mask INTC-1 -2 Edge/Level control
			IO (Decode16, 0xFD00, 0xFD00, 0x01, 0x40, ) // PM IOs
			IO (Decode16, 0xFE00, 0xFE00, 0x01, 0x10, ) // SMBUS IOs
			IO (Decode16, 0x0370, 0x0370, 0x01, 0x02, ) // ?? was 3F0, assuming its super IO ? So switched to 3F0
		})
		Return (BUF1)
	}
}

/* 8259-compatible Programmable Interrupt Controller */
Device (PIC)
{
	Name (_HID, EisaId ("PNP0000"))
	Name (_CRS, ResourceTemplate ()
	{
		IO (Decode16, 0x0020, 0x0020, 0x01, 0x02,)
		IO (Decode16, 0x00A0, 0x00A0, 0x01, 0x02,)
		IRQNoFlags () {2}
	})
}


/* PC-class DMA Controller */
Device (DMA1)
{
	Name (_HID, EisaId ("PNP0200"))
	Name (_CRS, ResourceTemplate ()
	{
		DMA (Compatibility, BusMaster, Transfer8,) {4}
		IO (Decode16, 0x0000, 0x0000, 0x01, 0x10,)
		IO (Decode16, 0x0080, 0x0080, 0x01, 0x11,)
		IO (Decode16, 0x0094, 0x0094, 0x01, 0x0C,)
		IO (Decode16, 0x00C0, 0x00C0, 0x01, 0x20,)
	})
}

/* PC-class System Timer */
Device (TMR)
{
	Name (_HID, EisaId ("PNP0100"))
	Name (_CRS, ResourceTemplate ()
	{
		IO (Decode16,0x0040,0x0040,0x01,0x04,)
		IRQNoFlags () {0}
	})
}

/* AT Real-Time Clock */
Device (RTC)
{
	Name (_HID, EisaId ("PNP0B00"))
	Name (_CRS, ResourceTemplate ()
	{
		IO (Decode16,0x0070,0x0070,0x01,0x04,)
		IRQNoFlags () {8}
	})
}

Device (SPKR)
{
	Name (_HID, EisaId ("PNP0800"))
	Name (_CRS, ResourceTemplate ()
	{
		IO (Decode16,0x0061,0x0061,0x01,0x01,)
	})
}

/* x87-compatible Floating Point Processing Unit */
Device (COPR)
{
	Name (_HID, EisaId ("PNP0C04"))
	Name (_CRS, ResourceTemplate ()
	{
		IO (Decode16,0x00F0,0x00F0,0x01,0x10,)
		IRQNoFlags () {13}
	})
}
