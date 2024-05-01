// GBK RFT20
#include <MCU/Intel/keil8051.h>
#include <MCU/delay.h>
#include <driver/DS18B20.h>
#include <driver/nixietube.h>
#include <driver/timer.h>
#include "intrins.h"
//#include "btnkey.h"

void delay(int ms)
{
	do delay001ms(); while(--ms);
}

char fin = 1;
float T,TShow;
unsigned char KeyNum;
unsigned long l;
void main()
{
	// 为什么 nixie 会有短暂的残影
	DS18B20_Convert();		//上电先转换一次温度，防止第一次读数据错误
	delay(1000);			//等待转换完成
	Timer0Init();
	
	while(1)
	{
		unsigned char i;
		unsigned long ll = l;
		for(i=0; i<4; i++) 
		{
			if( i==2) Nixie_Show(2, ll%10, 1);
			else Nixie_Show(i, ll%10, 0);
			delay(2);
			ll/=10;
		}
	}
}

void Timer0_Routine()
{
	static unsigned int T1Count;
	if(!fin) return;
	fin = 0;
	T1Count++;
	
	if(T1Count>=500)
	{
		T1Count = 0;
		/*温度读取及显示*/
		DS18B20_Convert();	//转换温度
		T=DS18B20_Read();	//读取温度
		l = T*100;
	}
	fin = 1;
} void (*RoutineINT0)(void) = Timer0_Routine;

