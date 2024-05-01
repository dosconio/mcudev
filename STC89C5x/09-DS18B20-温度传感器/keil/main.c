// GBK RFT20
#include <MCU/Intel/keil8051.h>
#include <MCU/delay.h>
#include <driver/DS18B20.h>
#include <driver/nixietube.h>
#include <driver/timer.h>
#include "intrins.h"
//#include "btnkey.h"

void delay(int ms)
{
	do delay001ms(); while(--ms);
}

char fin = 1;
float T,TShow;
unsigned char KeyNum;
unsigned long l;
void main()
{
	// Ϊʲô nixie ���ж��ݵĲ�Ӱ
	DS18B20_Convert();		//�ϵ���ת��һ���¶ȣ���ֹ��һ�ζ����ݴ���
	delay(1000);			//�ȴ�ת�����
	Timer0Init();
	
	while(1)
	{
		unsigned char i;
		unsigned long ll = l;
		for(i=0; i<4; i++) 
		{
			if( i==2) Nixie_Show(2, ll%10, 1);
			else Nixie_Show(i, ll%10, 0);
			delay(2);
			ll/=10;
		}
	}
}

void Timer0_Routine()
{
	static unsigned int T1Count;
	if(!fin) return;
	fin = 0;
	T1Count++;
	
	if(T1Count>=500)
	{
		T1Count = 0;
		/*�¶ȶ�ȡ����ʾ*/
		DS18B20_Convert();	//ת���¶�
		T=DS18B20_Read();	//��ȡ�¶�
		l = T*100;
	}
	fin = 1;
} void (*RoutineINT0)(void) = Timer0_Routine;

