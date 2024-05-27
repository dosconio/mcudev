#ifndef _LCD_INC_
#define _LCD_INC_

#include "MCU/ST/STM32F103VEx"
#include "Device/Video"

struct Col16 {
	static uint16 DColor(uni::DisplayColor dcolor) {
		// R5[11] G6[5] B5[0]
		uint16 r = dcolor.r >> (8-5), g = dcolor.g >> (8-6);
		uint16 res = (uint16)(dcolor.b >> (8-5));
		res |= g << 5;
		res |= r << 11;
		return res;
	}

	static uint16 ARGB(uint32 c32) {
		return DColor(uni::DisplayColorFrom32(c32));
	}
};

extern bool pressed;
extern uni::DisplayPoint CrtPoint;

extern uni::VideoControlBlock LCD;

void LCD_Initialize();

void LCD_Clear(stduint back = _COLOR_Silver);

#endif
