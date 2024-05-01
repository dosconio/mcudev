#include <MCU/Intel/keil8051.h>
#include <MCU/delay.h>
#include <driver/nixietube.h>

void main()
{
	while (1) 
	{
		Nixie_Show(0,0,0);
		Nixie_Show(1,1,0);
		Nixie_Show(2,2,0);
		Nixie_Show(3,3,0);
		Nixie_Show(4,4,1);
		Nixie_Show(5,5,1);
		Nixie_Show(6,6,1);
		Nixie_Show(7,7,1);
	}

	while (1)
		;
}