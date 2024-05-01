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
		BtnkeyLoop();//������������
	}
	T0Count2++;
	if(T0Count2>=2)
	{
		T0Count2=0;
		NixieBuffer[0]=Min/10;	//������ʾ���棬��ʾ����
		NixieBuffer[1]=Min%10;
		NixieBuffer[2]=16;
		NixieBuffer[3]=Sec/10;
		NixieBuffer[4]=Sec%10;
		NixieBuffer[5]=16;
		NixieBuffer[6]=MiniSec/10;
		NixieBuffer[7]=MiniSec%10;
		NixieLoop();//�������������
	}
	T0Count3++;
	if(T0Count3>=10)
	{
		T0Count3=0;
		Sec_Loop();//�����������
	}
} void (*RoutineINT0)(void) = RINT0;
void Bkey(unsigned char c)
{
	if(!c) return;
	switch (c)
	{
		case 1:
			RunFlag=!RunFlag; //������־λ��ת
			break;
		case 2:
			Min=0; //������0
			Sec=0;
			MiniSec=0;
			break;
		case 3:
			AT24C02_SetByte(0,Min); //������д��AT24C02
			delay(5);
			AT24C02_SetByte(1,Sec);
			delay(5);
			AT24C02_SetByte(2,MiniSec);
			delay(5);
			break;
		case 4:
			Min=AT24C02_GetByte(0); //����AT24C02����
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

