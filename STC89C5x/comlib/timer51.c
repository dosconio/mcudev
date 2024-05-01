// 20240429 dosconio
#include <stdinc.h>
#include <driver/timer.h>

// 对于 12MHz 每隔1us计数加一, 总共定时时间65535us, 64535离计数器溢出差值1000，所以计时时间为1ms

void Timer0Init(void)// 1ms@11.0592MHz 误差0.04% by STC-ISP (V6.91N)
{
	// AUXR &= 0x7F;// 定时器时钟12T模式
	pTMOD &= 0xF0;// 设置定时器模式
	pTMOD |= 0x01;// 设置定时器模式
	pTL0 = 0x66;// 设置定时初始值
	pTH0 = 0xFC;// 设置定时初始值
	pTCON_TF0 = 0;// 清除pTCON_TF0标志
	pTCON_TR0 = 1;// 定时器0开始计时
	pIE_ET0 = 1;
	pIE_EA = 1;
	pIP_PT0 = 0;
}

void Timer0SetCounter(unsigned int Value)
{
	pTH0 = Value / 256;
	pTL0 = Value % 256;
}

unsigned int Timer0GetCounter(void)
{
	return (pTH0 << 8) | pTL0;
}

void Timer0Mode(unsigned char Flag)
{
	pTCON_TR0 = Flag;
}

void Timer0Routine() interrupt 1// 1ms@11.0592MHz 误差0.04%
{
	// static unsigned int T0Count;
	pTL0 = 0x66;		//设置定时初值
	pTH0 = 0xFC;		//设置定时初值
	// T0Count++;
	if (RoutineINT0) RoutineINT0();
}// 貌似把 INT1 和 TIMER0 混淆了 TODO.
