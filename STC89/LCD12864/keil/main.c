#define _IMPLEMENT_KEIL8051
#include <MCU/8051/keil8051.h>
#include <MCU/delay.h>
#include <driver/LCD12864.h>

void main()
{
	// 20240305: cannot make true for:
	LCD12864_Initialize();
	LCD12864_Clear();
	LCD12864_CursorSet(0, 0);
	LCD12864_OutString("Hello,");
	LCD12864_CursorSet(1, 1);
	LCD12864_OutString("Keil!");
	LCD12864_Outi32hex(0x20240304);
	while (1)
		;
}