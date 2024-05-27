// GBK
#include <REGX51.H>
#include "norm.h"
#include "LCD1602.h"
#include "IR.h"
#define LCD1602ShowString(r,c,str) {LCD1602Cursor((c)-1,(r)-1);LCD1602PrintString(str);}
#define LCD1602ShowChar(r,c,chr) {LCD1602Cursor((c)-1,(r)-1);LCD1602PrintChar(chr);}
#define LCD1602ShowSignedNum(r,c,num,lim) {LCD1602Cursor((c)-1,(r)-1);LCD1602PrintSint8(num,lim+1);}
#define LCD1602ShowNum(r,c,num,lim) {LCD1602Cursor((c)-1,(r)-1);LCD1602PrintUint32(num,lim);}
#define LCD1602ShowHexNum(r,c,num,lim) {LCD1602Cursor((c)-1,(r)-1);LCD1602PrintUhex(num,lim);}
unsigned char Num;
unsigned char Address;
unsigned char Command;
void (*RoutineINT0)(void) = 0;
void main()
{
	LCD1602Init();
	LCD1602ShowString(1,1,"ADDR  CMD  NUM");
	LCD1602ShowString(2,1,"00    00   000");
	
	IR_Init();
	
	while(1)
	{
		if(IR_GetDataFlag() || IR_GetRepeatFlag())	//如果收到数据帧或者收到连发帧
		{
			Address=IR_GetAddress();		//获取遥控器地址码
			Command=IR_GetCommand();		//获取遥控器命令码
			
			LCD1602ShowHexNum(2,1,Address,2);	//显示遥控器地址码
			LCD1602ShowHexNum(2,7,Command,2);	//显示遥控器命令码
			
			if(Command==IR_VOL_MINUS)		//如果遥控器VOL-按键按下
			{
				Num--;						//Num自减
			}
			if(Command==IR_VOL_ADD)			//如果遥控器VOL+按键按下
			{
				Num++;						//Num自增
			}
			
			LCD1602ShowNum(2,12,Num,3);		//显示Num
		}
	}
}

