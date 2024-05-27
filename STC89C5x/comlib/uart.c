// GBK
#include <REGX51.H>

#include <driver/uart.h>

void UART_Init(void)// 9600bps@11.0592MHz
{
	PCON &= 0x7F;// 波特率不倍速
	SCON = 0x50;// 8位数据,可变波特率
	// AUXR &= 0xBF;// 定时器时钟12T模式
	// AUXR &= 0xFE;// 串口1选择定时器1为波特率发生器
	// AUXR: STC89C5x Do not have this
	TMOD &= 0x0F;// 设置定时器模式
	TMOD |= 0x20;// 设置定时器模式
	TL1 = 0xFD;// 设置定时初始值
	TH1 = 0xFD;// 设置定时重载值
	ET1 = 0;// 禁止定时器中断
	TR1 = 1;// 定时器1开始计时
	EA = 1;
	ES = 1;
}

void UART_Init11W(void)		//115200bps@11.0592MHz
{
	
	PCON &= 0x7F;// 波特率不倍速?
	SCON = 0x50;// 8位数据,可变波特率
	TMOD &= 0x0F;// 设置定时器模式
	TL1 = 0xFE;// 设置定时初始值
	TH1 = 0xFF;// 设置定时重载值
	ET1 = 0;// 禁止定时器中断
	TR1 = 1;// 定时器1开始计时
	EA = 1;
	ES = 1;
}


void UART_Out(unsigned char Byte)// send a byte to UART port
{
	SBUF = Byte;
	while (!TI);
	TI = 0;
}

void UARTRoutine() interrupt 4
{
	if (!RI) return;
	if (RoutineINT4) RoutineINT4();
	RI = 0;
}