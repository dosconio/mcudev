// GBK
#include "stdinc.h"
#include "btnkey.h"
#include <MCU/delay.h>

// Keil C Style
sbit K1 = p3 ^ 1;
sbit K2 = p3 ^ 0;
sbit K3 = p3 ^ 2;
sbit K4 = p3 ^ 3;

void (*EventBtnkeyDown)(unsigned char ki) = 0, (*EventBtnkeyUp)(unsigned char ki) = 0;
void (*EventBtnkeyEvent)(unsigned char ki) = 0;
unsigned char btnkey = 0;
const unsigned char BtnkeyCount = 4;

static void delay_Nms(byte ms)
{
	int i = 0;
	for(;i<ms;i++)
	{
		delay001ms();
	}
}

unsigned char Btnkey_Scan()
{
	unsigned char KeyNumber=0;
	if(p3_1==0){KeyNumber=1;}
	if(p3_0==0){KeyNumber=2;}
	if(p3_2==0){KeyNumber=3;}
	if(p3_3==0){KeyNumber=4;}
	return KeyNumber;
}

unsigned char BtnkeyScan()
{
	unsigned char KeyNumber=0;
	if(p3_1==0){delay_Nms(20);while(p3_1==0);delay_Nms(20);KeyNumber=1;}
	if(p3_0==0){delay_Nms(20);while(p3_0==0);delay_Nms(20);KeyNumber=2;}
	if(p3_2==0){delay_Nms(20);while(p3_2==0);delay_Nms(20);KeyNumber=3;}
	if(p3_3==0){delay_Nms(20);while(p3_3==0);delay_Nms(20);KeyNumber=4;}
	return KeyNumber;
}

void BtnkeyLoop()
{
	unsigned char crt = BtnkeyScan();
	if (!btnkey && crt)// down
	{
		if (EventBtnkeyDown) EventBtnkeyDown(crt);
	}
	else if (btnkey && !crt)// up
	{
		if (EventBtnkeyUp) EventBtnkeyUp(btnkey);
	}
	if (btnkey != crt && EventBtnkeyEvent) EventBtnkeyEvent(crt);
	btnkey = crt;
}
