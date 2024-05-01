// 20240429 dosconio
#include <stdinc.h>
#include <driver/timer.h>

// ���� 12MHz ÿ��1us������һ, �ܹ���ʱʱ��65535us, 64535������������ֵ1000�����Լ�ʱʱ��Ϊ1ms

void Timer0Init(void)// 1ms@11.0592MHz ���0.04% by STC-ISP (V6.91N)
{
	// AUXR &= 0x7F;// ��ʱ��ʱ��12Tģʽ
	pTMOD &= 0xF0;// ���ö�ʱ��ģʽ
	pTMOD |= 0x01;// ���ö�ʱ��ģʽ
	pTL0 = 0x66;// ���ö�ʱ��ʼֵ
	pTH0 = 0xFC;// ���ö�ʱ��ʼֵ
	pTCON_TF0 = 0;// ���pTCON_TF0��־
	pTCON_TR0 = 1;// ��ʱ��0��ʼ��ʱ
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

void Timer0Routine() interrupt 1// 1ms@11.0592MHz ���0.04%
{
	// static unsigned int T0Count;
	pTL0 = 0x66;		//���ö�ʱ��ֵ
	pTH0 = 0xFC;		//���ö�ʱ��ֵ
	// T0Count++;
	if (RoutineINT0) RoutineINT0();
}// ò�ư� INT1 �� TIMER0 ������ TODO.
