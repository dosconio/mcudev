#include <MCU/Intel/keil8051.h>
#include <MCU/delay.h>
#include <driver/timer.h>
#include <driver/AT24C02.h>
#include <driver/nixietube.h>
#include "intrins.h"
#include "btnkey.h"

void delay(int ms)
{
	do delay001ms(); while(--ms);
}

unsigned char Min,Sec,MiniSec;
unsigned char RunFlag;
static unsigned char NixieBuffer[8];
void NixieLoop()
{
	static unsigned nixcur = 0;
	unsigned char i;
	for(i=0; i<8; i++) Nixie_Show(7-i, NixieBuffer[i], 0);
	// average light, by Haruno RFT18
	Nixie_Show(7 - nixcur, NixieBuffer[nixcur], 0);
	nixcur++;
	if(nixcur >= 8) nixcur ^= nixcur;
}
void Sec_Loop(void)
{
	if(RunFlag)
	{
		MiniSec++;
		if(MiniSec>=100)
		{
			MiniSec=0;
			Sec++;
			if(Sec>=60)
			{
				Sec=0;
				Min++;
				if(Min>=60)
				{
					Min=0;
				}
			}
		}
	}
}

void RINT0(void)
{
	static unsigned int T0Count1,T0Count2,T0Count3;
	T0Count1++;
	if(T0Count1>=20)
	{
		T0Count1=0;
		BtnkeyLoop();//按键驱动函数
	}
	T0Count2++;
	if(T0Count2>=2)
	{
		T0Count2=0;
		NixieBuffer[0]=Min/10;	//设置显示缓存，显示数据
		NixieBuffer[1]=Min%10;
		NixieBuffer[2]=16;
		NixieBuffer[3]=Sec/10;
		NixieBuffer[4]=Sec%10;
		NixieBuffer[5]=16;
		NixieBuffer[6]=MiniSec/10;
		NixieBuffer[7]=MiniSec%10;
		NixieLoop();//数码管驱动函数
	}
	T0Count3++;
	if(T0Count3>=10)
	{
		T0Count3=0;
		Sec_Loop();//秒表驱动函数
	}
} void (*RoutineINT0)(void) = RINT0;
void Bkey(unsigned char c)
{
	if(!c) return;
	switch (c)
	{
		case 1:
			RunFlag=!RunFlag; //启动标志位翻转
			break;
		case 2:
			Min=0; //分秒清0
			Sec=0;
			MiniSec=0;
			break;
		case 3:
			AT24C02_SetByte(0,Min); //将分秒写入AT24C02
			delay(5);
			AT24C02_SetByte(1,Sec);
			delay(5);
			AT24C02_SetByte(2,MiniSec);
			delay(5);
			break;
		case 4:
			Min=AT24C02_GetByte(0); //读出AT24C02数据
			Sec=AT24C02_GetByte(1);
			MiniSec=AT24C02_GetByte(2);
			break;
	}
}

void main()
{
	EventBtnkeyEvent = Bkey;
	Timer0Init();
	while(1);
}

