

#include "../cominc/lcd.h"
extern "C" {
	#include "bsp_ili9341_lcd.h"
	#include "bsp_xpt2046_lcd.h"
}

using namespace uni;
// ---- core begin

class my_vci_t : public VideoControlInterface {
	virtual void SetCursor(const Point& disp) {}
	virtual Point GetCursor() { return Point(0,0); }
	virtual void DrawPoint (const Point& disp, Color* color) {
		if (color) LCD_SetTextColor(color->ToRGB565());
		ILI9341_SetPointPixel(disp.x, disp.y);
	}
	virtual void DrawRectangle (const DisplayRectangle& rect) {
		if (rect.filled) {
			ILI9341_OpenWindow (rect.x, rect.y, rect.width, rect.height);
			ILI9341_FillColor(rect.width * rect.height, rect.color.ToRGB565());
		}
		else {
			LCD.DrawLine(rect.getVertex(), 
				DisplaySize(rect.width, 1));
			LCD.DrawLine(Point(rect.x, rect.y + rect.height - 1), 
				DisplaySize(rect.width, 1));
			LCD.DrawLine(rect.getVertex(), 
				DisplaySize(1, rect.height));
			LCD.DrawLine(Point(rect.x + rect.width - 1, rect.y), 
				DisplaySize(1, rect.height));		
		}
	}
	virtual void DrawFont (const Point& disp, const DisplayFont& font) {
		ILI9341_DispString_EN(disp.x, disp.y, (char*)font.addr);
	}
	virtual Color GetColor(Point p){return Color::From32(0);}
} my_vci;

VideoControlBlock LCD(0, &my_vci);

// ---- core end

void LCD_Initialize() {
	ILI9341_Init(); XPT2046_Init(); ILI9341_GramScan(5);
	LCD.setMode(0 /*TODO*/, LCD_X_LENGTH, LCD_Y_LENGTH);
}

void LCD_Clear(stduint back) {
	LCD.Draw(Rectangle(Point(0, 0), Size2(LCD_X_LENGTH, LCD_Y_LENGTH),
		Color::From32(back), true));
}

