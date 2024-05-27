// GBK RFT20
#include <MCU/Intel/keil8051.h>
#include <MCU/delay.h>
#include <driver/DS18B20.h>
#include <driver/nixietube.h>
#include <driver/timer.h>
#include "intrins.h"
#include "mtrled.h"
//#include "btnkey.h"

void delay(int ms)
{
	do delay001ms(); while(--ms);
}

byte gled_row[8]={0x38,0x7C,0x7E,0x3F,0x3F,0x7E,0x7C,0x38};//LED������ʾͼ���������
byte gled_col[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};//LED������ʾͼ���������
void main()
{
	byte i=0;
	MtrLEDInit();
	while(1)
	{			
		for(i=0;i<8;i++)//ѭ��8��ɨ��8�С���
		{
			//LEDDZ_COL_PORT=gled_col[i];//������ѡ����
			MtrLEDSetCol(i, gled_row[i]);//������ѡ����
			delay(10);//��ʱһ��ʱ�䣬�ȴ���ʾ�ȶ�
			MtrLEDSetCol(i, 0x00);//��Ӱ	
		}								
	}
}


