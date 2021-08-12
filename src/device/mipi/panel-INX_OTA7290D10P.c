/* SPDX-License-Identifier: GPL-2.0-only */

#include <device/mipi_panel.h>

struct panel_serializable_data INX_OTA7290D10P = {
	.edid = {
		.ascii_string = "OTA7290D10P",
		.manufacturer_name = "INX",
		.panel_bits_per_color = 8,
		.panel_bits_per_pixel = 24,
		.mode = {
			.pixel_clock = 159420,
			.lvds_dual_channel = 0,
			.refresh = 60,
			.ha = 1200, .hbl = 141, .hso = 80, .hspw = 1,
			.va = 1920, .vbl = 61, .vso = 35, .vspw = 1,
			.phsync = '-', .pvsync = '-',
			.x_mm = 135, .y_mm = 216,
		},
	},
	.init = {
		PANEL_DCS(0xB0, 0x5A),
		PANEL_DCS(0xB1, 0x00),
		PANEL_DCS(0x89, 0x01),
		PANEL_DCS(0x91, 0x17),
		PANEL_DCS(0xB1, 0x03),
		PANEL_DCS(0x2C, 0x28),
		PANEL_DCS(0x00, 0xF1),
		PANEL_DCS(0x01, 0x78),
		PANEL_DCS(0x02, 0x3C),
		PANEL_DCS(0x03, 0x1E),
		PANEL_DCS(0x04, 0x8F),
		PANEL_DCS(0x05, 0x01),
		PANEL_DCS(0x06, 0x00),
		PANEL_DCS(0x07, 0x00),
		PANEL_DCS(0x08, 0x00),
		PANEL_DCS(0x09, 0x00),
		PANEL_DCS(0x0A, 0x01),
		PANEL_DCS(0x0B, 0x3C),
		PANEL_DCS(0x0C, 0x00),
		PANEL_DCS(0x0D, 0x00),
		PANEL_DCS(0x0E, 0x24),
		PANEL_DCS(0x0F, 0x1C),
		PANEL_DCS(0x10, 0xC8),
		PANEL_DCS(0x11, 0x60),
		PANEL_DCS(0x12, 0x70),
		PANEL_DCS(0x13, 0x01),
		PANEL_DCS(0x14, 0xE3),
		PANEL_DCS(0x15, 0xFF),
		PANEL_DCS(0x16, 0x3D),
		PANEL_DCS(0x17, 0x0E),
		PANEL_DCS(0x18, 0x01),
		PANEL_DCS(0x19, 0x00),
		PANEL_DCS(0x1A, 0x00),
		PANEL_DCS(0x1B, 0xFC),
		PANEL_DCS(0x1C, 0x0B),
		PANEL_DCS(0x1D, 0xA0),
		PANEL_DCS(0x1E, 0x03),
		PANEL_DCS(0x1F, 0x04),
		PANEL_DCS(0x20, 0x0C),
		PANEL_DCS(0x21, 0x00),
		PANEL_DCS(0x22, 0x04),
		PANEL_DCS(0x23, 0x81),
		PANEL_DCS(0x24, 0x1F),
		PANEL_DCS(0x25, 0x10),
		PANEL_DCS(0x26, 0x9B),
		PANEL_DCS(0x2D, 0x01),
		PANEL_DCS(0x2E, 0x84),
		PANEL_DCS(0x2F, 0x00),
		PANEL_DCS(0x30, 0x02),
		PANEL_DCS(0x31, 0x08),
		PANEL_DCS(0x32, 0x01),
		PANEL_DCS(0x33, 0x1C),
		PANEL_DCS(0x34, 0x70),
		PANEL_DCS(0x35, 0xFF),
		PANEL_DCS(0x36, 0xFF),
		PANEL_DCS(0x37, 0xFF),
		PANEL_DCS(0x38, 0xFF),
		PANEL_DCS(0x39, 0xFF),
		PANEL_DCS(0x3A, 0x05),
		PANEL_DCS(0x3B, 0x00),
		PANEL_DCS(0x3C, 0x00),
		PANEL_DCS(0x3D, 0x00),
		PANEL_DCS(0x3E, 0x0F),
		PANEL_DCS(0x3F, 0xA4),
		PANEL_DCS(0x40, 0x28),
		PANEL_DCS(0x41, 0xFC),
		PANEL_DCS(0x42, 0x01),
		PANEL_DCS(0x43, 0x08),
		PANEL_DCS(0x44, 0x05),
		PANEL_DCS(0x45, 0xF0),
		PANEL_DCS(0x46, 0x01),
		PANEL_DCS(0x47, 0x02),
		PANEL_DCS(0x48, 0x00),
		PANEL_DCS(0x49, 0x58),
		PANEL_DCS(0x4A, 0x00),
		PANEL_DCS(0x4B, 0x05),
		PANEL_DCS(0x4C, 0x03),
		PANEL_DCS(0x4D, 0xD0),
		PANEL_DCS(0x4E, 0x13),
		PANEL_DCS(0x4F, 0xFF),
		PANEL_DCS(0x50, 0x0A),
		PANEL_DCS(0x51, 0x53),
		PANEL_DCS(0x52, 0x26),
		PANEL_DCS(0x53, 0x22),
		PANEL_DCS(0x54, 0x09),
		PANEL_DCS(0x55, 0x22),
		PANEL_DCS(0x56, 0x00),
		PANEL_DCS(0x57, 0x1C),
		PANEL_DCS(0x58, 0x03),
		PANEL_DCS(0x59, 0x3F),
		PANEL_DCS(0x5A, 0x28),
		PANEL_DCS(0x5B, 0x01),
		PANEL_DCS(0x5C, 0xCC),
		PANEL_DCS(0x5D, 0x21),
		PANEL_DCS(0x5E, 0x04),
		PANEL_DCS(0x5F, 0x13),
		PANEL_DCS(0x60, 0x42),
		PANEL_DCS(0x61, 0x08),
		PANEL_DCS(0x62, 0x64),
		PANEL_DCS(0x63, 0xEB),
		PANEL_DCS(0x64, 0x10),
		PANEL_DCS(0x65, 0xA8),
		PANEL_DCS(0x66, 0x84),
		PANEL_DCS(0x67, 0x8E),
		PANEL_DCS(0x68, 0x29),
		PANEL_DCS(0x69, 0x11),
		PANEL_DCS(0x6A, 0x42),
		PANEL_DCS(0x6B, 0x38),
		PANEL_DCS(0x6C, 0x21),
		PANEL_DCS(0x6D, 0x84),
		PANEL_DCS(0x6E, 0x50),
		PANEL_DCS(0x6F, 0xB6),
		PANEL_DCS(0x70, 0x0E),
		PANEL_DCS(0x71, 0xA1),
		PANEL_DCS(0x72, 0xCE),
		PANEL_DCS(0x73, 0xF8),
		PANEL_DCS(0x74, 0xDA),
		PANEL_DCS(0x75, 0x1A),
		PANEL_DCS(0x76, 0x00),
		PANEL_DCS(0x77, 0x00),
		PANEL_DCS(0x78, 0x5F),
		PANEL_DCS(0x79, 0xE0),
		PANEL_DCS(0x7A, 0x01),
		PANEL_DCS(0x7B, 0xFF),
		PANEL_DCS(0x7C, 0xFF),
		PANEL_DCS(0x7D, 0xFF),
		PANEL_DCS(0x7E, 0xFF),
		PANEL_DCS(0x7F, 0xFE),
		PANEL_DCS(0xB1, 0x02),
		PANEL_DCS(0x00, 0xFF),
		PANEL_DCS(0x01, 0x01),
		PANEL_DCS(0x02, 0x00),
		PANEL_DCS(0x03, 0x00),
		PANEL_DCS(0x04, 0x00),
		PANEL_DCS(0x05, 0x00),
		PANEL_DCS(0x06, 0x00),
		PANEL_DCS(0x07, 0x00),
		PANEL_DCS(0x08, 0xC0),
		PANEL_DCS(0x09, 0x00),
		PANEL_DCS(0x0A, 0x00),
		PANEL_DCS(0x0B, 0x04),
		PANEL_DCS(0x0C, 0xE6),
		PANEL_DCS(0x0D, 0x0D),
		PANEL_DCS(0x0F, 0x08),
		PANEL_DCS(0x10, 0xE5),
		PANEL_DCS(0x11, 0xA8),
		PANEL_DCS(0x12, 0xEC),
		PANEL_DCS(0x13, 0x54),
		PANEL_DCS(0x14, 0x5A),
		PANEL_DCS(0x15, 0xD5),
		PANEL_DCS(0x16, 0x23),
		PANEL_DCS(0x17, 0x11),
		PANEL_DCS(0x18, 0x2F),
		PANEL_DCS(0x19, 0x93),
		PANEL_DCS(0x1A, 0xA6),
		PANEL_DCS(0x1B, 0x0F),
		PANEL_DCS(0x1C, 0xFF),
		PANEL_DCS(0x1D, 0xFF),
		PANEL_DCS(0x1E, 0xFF),
		PANEL_DCS(0x1F, 0xFF),
		PANEL_DCS(0x20, 0xFF),
		PANEL_DCS(0x21, 0xFF),
		PANEL_DCS(0x22, 0xFF),
		PANEL_DCS(0x23, 0xFF),
		PANEL_DCS(0x24, 0xFF),
		PANEL_DCS(0x25, 0xFF),
		PANEL_DCS(0x26, 0xFF),
		PANEL_DCS(0x27, 0x1F),
		PANEL_DCS(0x28, 0xC8),
		PANEL_DCS(0x29, 0xFF),
		PANEL_DCS(0x2A, 0xFF),
		PANEL_DCS(0x2B, 0xFF),
		PANEL_DCS(0x2C, 0x07),
		PANEL_DCS(0x2D, 0x03),
		PANEL_DCS(0x33, 0x09),
		PANEL_DCS(0x35, 0x7F),
		PANEL_DCS(0x36, 0x0C),
		PANEL_DCS(0x38, 0x7F),
		PANEL_DCS(0x3A, 0x80),
		PANEL_DCS(0x3B, 0x55),
		PANEL_DCS(0x3C, 0xE2),
		PANEL_DCS(0x3D, 0x32),
		PANEL_DCS(0x3E, 0x00),
		PANEL_DCS(0x3F, 0x58),
		PANEL_DCS(0x40, 0x06),
		PANEL_DCS(0x41, 0x80),
		PANEL_DCS(0x42, 0xCB),
		PANEL_DCS(0x43, 0x2C),
		PANEL_DCS(0x44, 0x61),
		PANEL_DCS(0x45, 0x39),
		PANEL_DCS(0x46, 0x00),
		PANEL_DCS(0x47, 0x00),
		PANEL_DCS(0x48, 0x8B),
		PANEL_DCS(0x49, 0xD2),
		PANEL_DCS(0x4A, 0x01),
		PANEL_DCS(0x4B, 0x00),
		PANEL_DCS(0x4C, 0x10),
		PANEL_DCS(0x4D, 0xC0),
		PANEL_DCS(0x4E, 0x0F),
		PANEL_DCS(0x4F, 0xF1),
		PANEL_DCS(0x50, 0x78),
		PANEL_DCS(0x51, 0x7A),
		PANEL_DCS(0x52, 0x34),
		PANEL_DCS(0x53, 0x99),
		PANEL_DCS(0x54, 0xA2),
		PANEL_DCS(0x55, 0x03),
		PANEL_DCS(0x56, 0x6C),
		PANEL_DCS(0x57, 0x1A),
		PANEL_DCS(0x58, 0x05),
		PANEL_DCS(0x59, 0x30),
		PANEL_DCS(0x5A, 0x1E),
		PANEL_DCS(0x5B, 0x8F),
		PANEL_DCS(0x5C, 0xC7),
		PANEL_DCS(0x5D, 0xE3),
		PANEL_DCS(0x5E, 0xF1),
		PANEL_DCS(0x5F, 0x78),
		PANEL_DCS(0x60, 0x3C),
		PANEL_DCS(0x61, 0x36),
		PANEL_DCS(0x62, 0x1E),
		PANEL_DCS(0x63, 0x1B),
		PANEL_DCS(0x64, 0x8F),
		PANEL_DCS(0x65, 0xC7),
		PANEL_DCS(0x66, 0xE3),
		PANEL_DCS(0x67, 0x31),
		PANEL_DCS(0x68, 0x14),
		PANEL_DCS(0x69, 0x89),
		PANEL_DCS(0x6A, 0x70),
		PANEL_DCS(0x6B, 0x8C),
		PANEL_DCS(0x6C, 0x8D),
		PANEL_DCS(0x6D, 0x8D),
		PANEL_DCS(0x6E, 0x8D),
		PANEL_DCS(0x6F, 0x8D),
		PANEL_DCS(0x70, 0xC7),
		PANEL_DCS(0x71, 0xE3),
		PANEL_DCS(0x72, 0xF1),
		PANEL_DCS(0x73, 0xD8),
		PANEL_DCS(0x74, 0xD8),
		PANEL_DCS(0x75, 0xD8),
		PANEL_DCS(0x76, 0x18),
		PANEL_DCS(0x77, 0x00),
		PANEL_DCS(0x78, 0x00),
		PANEL_DCS(0x79, 0x00),
		PANEL_DCS(0x7A, 0xC6),
		PANEL_DCS(0x7B, 0xC6),
		PANEL_DCS(0x7C, 0xC6),
		PANEL_DCS(0x7D, 0xC6),
		PANEL_DCS(0x7E, 0xC6),
		PANEL_DCS(0x7F, 0xE3),
		PANEL_DCS(0x0B, 0x04),
		PANEL_DCS(0xB1, 0x03),
		PANEL_DCS(0x2C, 0x2C),
		PANEL_DCS(0xB1, 0x00),
		PANEL_DCS(0x89, 0x03),
		PANEL_DCS(0x11),
		PANEL_DELAY(0x78),
		PANEL_DCS(0x29),
		PANEL_DELAY(0x14),
		PANEL_END,
	},
};
