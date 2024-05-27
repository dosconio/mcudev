#include <MCU/Intel/keil8051.h>
#include <MCU/delay.h>
#include <driver/nixietube.h>
#include <driver/uart.h>
#include <REGX51.H>

void uart_init(u8 baud)
{
	TMOD|=0X20;	//设置计数器工作方式2
	SCON=0X50;	//设置为工作方式1
	PCON=0X80;	//波特率加倍
	TH1=baud;	//计数器初始值设置
	TL1=baud;
	ES=1;		//打开接收中断
	EA=1;		//打开总中断
	TR1=1;		//打开计数器		
}

void main()
{
	//UART_Init();
	uart_init(0xFA);// 9600
	while (1)
		;
}

void RINT4(void)
{
	byte rec_data;

	RI = 0;			//清除接收中断标志位
	rec_data=SBUF;	//存储接收到的数据
	SBUF=rec_data;	//将接收到的数据放入到发送寄存器
	while(!TI);		//等待发送数据完成
	TI=0;			//清除发送完成标志位	
} void (*RoutineINT4)(void) = RINT4;
