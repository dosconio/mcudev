#include <stdinc.h>

sbit LED=p2^0;

void Delay(unsigned int t)
{
	while(t--);
}

void main()
{
	unsigned char Time,i;
	while(1)
	{
		for(Time=0;Time<100;Time++)// to light
		{
			for(i=0;i<20;i++)
			{
				LED=0;
				Delay(Time);
				LED=1;
				Delay(100-Time);
			}
		}
		for(Time=100;Time>0;Time--)// to dark
		{
			for(i=0;i<20;i++)
			{
				LED=0;
				Delay(Time);
				LED=1;
				Delay(100-Time);
			}
		}
	}
}
