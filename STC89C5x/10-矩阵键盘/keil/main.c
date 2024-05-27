#include <MCU/Intel/keil8051.h>
#include <MCU/delay.h>
#include <driver/nixietube.h>
#include "mtrkey.h"

void main()
{
	int i = 0;
	while (1) 
	{
		uint16 map = MtrKeyReadMap();
		for (i=0; i<4; i++) Nixie_Show(i,(map>>4*i) & 0xF,0);
		Nixie_Show(0,0x10,0);
		delay001ms();
	}

	while (1)
		;
}