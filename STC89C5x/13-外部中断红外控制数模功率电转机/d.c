// GBK
#include <REGX52.H>
#include "norm.h"
#include "btnkey.h"
#include "nixie.h"


sbit Motor=P1^0;

unsigned char Counter,Compare;	//计数值和比较值，用于输出PWM
unsigned char KeyNum,Speed;
void Timer0Init(void)// 1ms@11.0592MHz 误差0.04% by STC-ISP (V6.91N)
{
	// AUXR &= 0x7F;// 定时器时钟12T模式
	TMOD &= 0xF0;// 设置定时器模式
	TMOD |= 0x01;// 设置定时器模式
	TL0 = 0x66;// 设置定时初始值
	TH0 = 0xFC;// 设置定时初始值
	TF0 = 0;// 清除TF0标志
	TR0 = 1;// 定时器0开始计时
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
			if(Speed==0){Compare=0;}	//设置比较值，改变PWM占空比
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
	// 注意 LED负逻辑 与 电机正逻辑 相反
	if(Counter<Compare)
	{
		Motor=1;
	}
	else
	{
		Motor=0;
	}
}
