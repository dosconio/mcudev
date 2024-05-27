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

byte gled_row[8]={0x38,0x7C,0x7E,0x3F,0x3F,0x7E,0x7C,0x38};//LED点阵显示图像的行数据
byte gled_col[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};//LED点阵显示图像的列数据
void main()
{
	byte i=0;
	MtrLEDInit();
	while(1)
	{			
		for(i=0;i<8;i++)//循环8次扫描8行、列
		{
			//LEDDZ_COL_PORT=gled_col[i];//传送列选数据
			MtrLEDSetCol(i, gled_row[i]);//传送行选数据
			delay(10);//延时一段时间，等待显示稳定
			MtrLEDSetCol(i, 0x00);//消影	
		}								
	}
}


