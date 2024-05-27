// GBK
#include <REGX51.H>
#include "norm.h"
#include "LCD1602.h"
#include "XPT2046.h"
#define LCD1602ShowString(r,c,str) {LCD1602Cursor((c)-1,(r)-1);LCD1602PrintString(str);}
#define LCD1602ShowChar(r,c,chr) {LCD1602Cursor((c)-1,(r)-1);LCD1602PrintChar(chr);}
#define LCD1602ShowSignedNum(r,c,num,lim) {LCD1602Cursor((c)-1,(r)-1);LCD1602PrintSint8(num,lim+1);}
#define LCD1602ShowNum(r,c,num,lim) {LCD1602Cursor((c)-1,(r)-1);LCD1602PrintUint32(num,lim);}
unsigned int ADValue;

void main(void)
{
	LCD1602Init();
	LCD1602ShowString(1,1,"ADJ  NTC  GR");
	while(1)
	{
		ADValue=XPT2046ReadAD(XPT2046_XP);// 读取AIN0，可调电阻
		LCD1602ShowNum(2,1,ADValue,3);
		ADValue=XPT2046ReadAD(XPT2046_YP);// 读取AIN1，热敏电阻
		LCD1602ShowNum(2,6,ADValue,3);
		ADValue=XPT2046ReadAD(XPT2046_VBAT);// 读取AIN2，光敏电阻
		LCD1602ShowNum(2,11,ADValue,3);	
		delay(100);
	}
}
