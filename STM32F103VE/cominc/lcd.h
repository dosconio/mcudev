#ifndef _LCD_INC_
#define _LCD_INC_

#include "MCU/ST/STM32F103VEx"
#include "Device/Video"


extern bool pressed;
extern uni::Point CrtPoint;

extern uni::VideoControlBlock LCD;

void LCD_Initialize();

void LCD_Clear(stduint back = uni::Color::Silver);

void LCD_SetColors(uni::Color::ColorIdentifier TColor, uni::Color::ColorIdentifier BColor);
void LCD_SetColors_TODE(uint16_t TextColor, uint16_t BackColor);

#endif
