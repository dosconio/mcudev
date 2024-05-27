#include <MCU/Intel/keil8051.h>
#include <MCU/delay.h>
#include <driver/nixietube.h>
#include <driver/uart.h>
#include <REGX51.H>

void uart_init(u8 baud)
{
	TMOD|=0X20;	//���ü�����������ʽ2
	SCON=0X50;	//����Ϊ������ʽ1
	PCON=0X80;	//�����ʼӱ�
	TH1=baud;	//��������ʼֵ����
	TL1=baud;
	ES=1;		//�򿪽����ж�
	EA=1;		//�����ж�
	TR1=1;		//�򿪼�����		
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

	RI = 0;			//��������жϱ�־λ
	rec_data=SBUF;	//�洢���յ�������
	SBUF=rec_data;	//�����յ������ݷ��뵽���ͼĴ���
	while(!TI);		//�ȴ������������
	TI=0;			//���������ɱ�־λ	
} void (*RoutineINT4)(void) = RINT4;
