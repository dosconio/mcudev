

#include "../cominc/lcd.h"
extern "C" {
	#include "bsp_ili9341_lcd.h"
	#include "bsp_xpt2046_lcd.h"
}

using namespace uni;
// ---- core begin
VideoControlBlock LCD(0);

//void doSetCursor(DisplayPoint disp) {}
//DisplayPoint doGetCursor() {}
void doDrawPoint (DisplayPoint disp, DisplayColor* color) {
	if (color) LCD_SetTextColor(Col16::DColor(*color));
	ILI9341_SetPointPixel(disp.x, disp.y);
}
void doDrawRectangle (DisplayRectangle rect) {
	if (rect.filled) {
		ILI9341_OpenWindow (rect.x, rect.y, rect.width, rect.height);
		ILI9341_FillColor(rect.width * rect.height, Col16::DColor(rect.color));
	}
	else {
		LCD.DrawLine(rect.getVertex(), 
			DisplaySize(rect.width, 1));
		LCD.DrawLine(DisplayPoint(rect.x, rect.y + rect.height - 1), 
			DisplaySize(rect.width, 1));
		LCD.DrawLine(rect.getVertex(), 
			DisplaySize(1, rect.height));
		LCD.DrawLine(DisplayPoint(rect.x + rect.width - 1, rect.y), 
			DisplaySize(1, rect.height));		
	}
}
void doDrawFont (DisplayPoint disp, DisplayFont font) {
	ILI9341_DispString_EN(disp.x, disp.y, (char*)font.addr);
}

// ---- core end

void LCD_Initialize() {
	ILI9341_Init(); XPT2046_Init(); ILI9341_GramScan(5);
	LCD.setMode(0 /*TODO*/, LCD_X_LENGTH, LCD_Y_LENGTH);
	LCD.setBind(0, 0, doDrawPoint, doDrawRectangle, doDrawFont);
}

void LCD_Clear(stduint back) {
	DisplayRectangle fullscr = {
		0, 0, LCD_X_LENGTH, LCD_Y_LENGTH,
		DisplayColorFrom32(back), true
	};
	LCD.Draw(fullscr);
}

