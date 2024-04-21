
#include <MCU/Intel/sdcc8051.h> // #include <mcs51/8051.h>
#include <MCU/delay.h>
#include <MCU/port.h>

#define delaya                    \
	for (byte i = 0; i < 0xFA; i++) \
		delay001ms()

void LED_SingleRun(byte logic);

void main() 
{
	while (1)
	{
		LED_SingleRun(0);
		LED_SingleRun(1);
	}
}

void LED_SingleRun(byte logic)
{
	byte stream = 1;
	byte direction = 0;
	while (1)
	{
		outpb(2, logic ? stream : ~stream);
		delaya;
		if (direction)
		{
			if (stream >>= 1); else
			{
				direction = 0, stream = 1;
				break;
			}
		}
		else
		{
			if (!(stream <<= 1))
				direction = 1, stream = 0b01000000;
		}
	}
	outpb(2, logic ? stream : ~stream);
}
