#include <REGX51.H>
#include "../norm.h"
#include "../mtrled.h"

#define PORT_MtrLED P0
sbit RCK = P3 ^ 5;// RCLK
sbit SCK = P3 ^ 6;// SRCLK
sbit SER = P3 ^ 4;// SER

static void Chip74HC595WriteByte(unsigned char Byte)
{
	unsigned char i;
	for (i = 0;i < 8;i++)
	{
		SER = Byte & (0x80 >> i);
		SCK = 1;
		SCK = 0;
	}
	RCK = 1;
	RCK = 0;
}

void MtrLEDInit()
{
	SCK=0;
	RCK=0;
}

void MtrLEDSetCol(unsigned char col, unsigned char byt)
{
	Chip74HC595WriteByte(byt);// data is a keyword
	PORT_MtrLED = ~(0x80 >> col);
	delay(1);
	PORT_MtrLED = 0xFF;
}

