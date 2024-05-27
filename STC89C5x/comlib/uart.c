// GBK
#include <REGX51.H>

#include <driver/uart.h>

void UART_Init(void)// 9600bps@11.0592MHz
{
	PCON &= 0x7F;// �����ʲ�����
	SCON = 0x50;// 8λ����,�ɱ䲨����
	// AUXR &= 0xBF;// ��ʱ��ʱ��12Tģʽ
	// AUXR &= 0xFE;// ����1ѡ��ʱ��1Ϊ�����ʷ�����
	// AUXR: STC89C5x Do not have this
	TMOD &= 0x0F;// ���ö�ʱ��ģʽ
	TMOD |= 0x20;// ���ö�ʱ��ģʽ
	TL1 = 0xFD;// ���ö�ʱ��ʼֵ
	TH1 = 0xFD;// ���ö�ʱ����ֵ
	ET1 = 0;// ��ֹ��ʱ���ж�
	TR1 = 1;// ��ʱ��1��ʼ��ʱ
	EA = 1;
	ES = 1;
}

void UART_Init11W(void)		//115200bps@11.0592MHz
{
	
	PCON &= 0x7F;// �����ʲ�����?
	SCON = 0x50;// 8λ����,�ɱ䲨����
	TMOD &= 0x0F;// ���ö�ʱ��ģʽ
	TL1 = 0xFE;// ���ö�ʱ��ʼֵ
	TH1 = 0xFF;// ���ö�ʱ����ֵ
	ET1 = 0;// ��ֹ��ʱ���ж�
	TR1 = 1;// ��ʱ��1��ʼ��ʱ
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