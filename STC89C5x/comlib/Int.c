#include <REGX51.H>

void INT0Init(void)
{
	IT0=1;
	IE0=0;
	EX0=1;
	EA=1;
	PX0=1;
}

/* Routine
void Int0_Routine(void) interrupt 0
{
	
}
*/
