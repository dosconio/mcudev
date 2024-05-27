// GBK
#include <REGX52.H>
#include "norm.h"
#include "btnkey.h"
#include "nixie.h"


sbit Motor=P1^0;

unsigned char Counter,Compare;	//����ֵ�ͱȽ�ֵ���������PWM
unsigned char KeyNum,Speed;
void Timer0Init(void)// 1ms@11.0592MHz ���0.04% by STC-ISP (V6.91N)
{
	// AUXR &= 0x7F;// ��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;// ���ö�ʱ��ģʽ
	TMOD |= 0x01;// ���ö�ʱ��ģʽ
	TL0 = 0x66;// ���ö�ʱ��ʼֵ
	TH0 = 0xFC;// ���ö�ʱ��ʼֵ
	TF0 = 0;// ���TF0��־
	TR0 = 1;// ��ʱ��0��ʼ��ʱ
	ET0=1;
	EA=1;
	PT0=0;
}
void main()
{
	Timer0Init();
	while(1)
	{
		KeyNum=BtnkeyScan();
		if(KeyNum==1)
		{
			Speed++;
			Speed%=4;
			if(Speed==0){Compare=0;}	//���ñȽ�ֵ���ı�PWMռ�ձ�
			if(Speed==1){Compare=50;}
			if(Speed==2){Compare=75;}
			if(Speed==3){Compare=100;}
		}
		NixieSet(0,Speed);
		delay(50);
	}
}
void Timer0_Routine() interrupt 1
{
	TL0 = 0x9C;
	TH0 = 0xFF;
	Counter++;
	Counter%=100;
	// ע�� LED���߼� �� ������߼� �෴
	if(Counter<Compare)
	{
		Motor=1;
	}
	else
	{
		Motor=0;
	}
}
