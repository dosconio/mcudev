#define _IMPLEMENT_KEIL8051
#include <MCU/8051/keil8051.h>
#include <MCU/delay.h>
#include <driver/LCD12864.h>

void main()
{
	// 20240305: cannot make true for:
	// 20240413: ԭ���Ǳ��߰Ѳ��� LCD12864 װ����
	LCD12864_Initialize();
	LCD12864_Clear();
	LCD12864_CursorSet(1, 0);// (COL,ROW)
	LCD12864_OutString("Hello,");
	LCD12864_CursorSet(0, 1);// Second Line
	LCD12864_OutString("��˥å���ܥ��");
	LCD12864_Outi32hex(0x20240304);
	while (1)
		;
}