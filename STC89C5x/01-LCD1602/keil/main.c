#define _IMPLEMENT_KEIL8051
#include <MCU/8051/keil8051.h>
#include <MCU/delay.h>
#include <driver/LCD1602.h>
#include "intrins.h"
void main()
{
	LCD1602_Initialize();
	LCD1602_Clear();
	LCD1602_CursorSet(0, 0);
	LCD1602_OutString("Hello,");
	LCD1602_CursorSet(1, 1);
	LCD1602_OutString("Keil!");
	LCD1602_Outi32hex(0x20240304);
	while (1)
		;
}