#include <stm32f10x.h>
#include "ADF4001.h"
#include "Delay.h"



//====================================================================================
// 初始化程序区
//====================================================================================

//====================================================================================
// void GPIO_ADF4001_Configuration(void)
// 函数功能: ADF4001引脚配置函数
// 入口参数: 无
// 返回参数: 无
// 全局变量: 无
// 调用模块: RCC_APB2PeriphClockCmd(); GPIO_Init();
// 注意事项: 无
//====================================================================================
void GPIO_ADF4001_Configuration(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;

	// 使能IO口时钟
	RCC_APB2PeriphClockCmd(  RCC_ADF4001_CE , ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = GPIO_ADF4001_Pin_CE; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			 // 推挽输出			 
	GPIO_Init(GPIO_ADF4001_Port_CE, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(  RCC_ADF4001_LE , ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = GPIO_ADF4001_Pin_LE; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			 // 推挽输出			 
	GPIO_Init(GPIO_ADF4001_Port_LE, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(  RCC_ADF4001_CLK , ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = GPIO_ADF4001_Pin_CLK; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			 // 推挽输出			 
	GPIO_Init(GPIO_ADF4001_Port_CLK, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(  RCC_ADF4001_DATA , ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = GPIO_ADF4001_Pin_DATA; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			 // 推挽输出			 
	GPIO_Init(GPIO_ADF4001_Port_DATA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(  RCC_ADF4001_MOUT , ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Pin = GPIO_ADF4001_Pin_MOUT;
	GPIO_Init(GPIO_ADF4001_Port_MOUT ,&GPIO_InitStructure);

}


void ADF4001_SetFre(u16 F100k,u32 lockmode)
{
	
	ADF4001_WR_Data(0x1f8093);			//初始化
	Delay_ns(10);
	ADF4001_WR_Data(0x000000 | ADF4001LO<< 2 );	//30Mhz本振，所以是300分频为100Khz基础单位！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！1
	Delay_ns(10);
	ADF4001_WR_Data(0x200001 | F100k<< 8 );	//100khz*1000倍频为1Mhz
	Delay_ns(10);
	ADF4001_WR_Data(lockmode);	    //锁定时间测量
	
}

//====================================================================================
//函数名称:void ADF4001_Init(void)
//函数功能:ADF4001初始化
//入口参数:无
//出口参数:无
//====================================================================================
void ADF4001_Init(void)
{
	GPIO_ADF4001_Configuration();       
	Delay_1us(10)   ;
	
	ADF4001_CE_Set  ;
	Delay_1us(5)    ;
	ADF4001_LE_Clr  ;
	ADF4001_CLK_Clr ;
	ADF4001_DATA_Clr;	
	Delay_1us(1)    ;
	
	ADF4001_SetFre(1000,LockDetectMode_D);
	
}

//====================================================================================
// ADF4001驱动区
//====================================================================================

//====================================================================================
//函数名称: void ADF4001_WR_Data(unsigned long int data)
//函数功能: ADF4001写入数据
//入口参数: data: 24bit 数据														
//出口参数:无
//====================================================================================
void ADF4001_WR_Data(unsigned long int data)
{
	u8 i ;
	unsigned long int temp;
	
	temp = 0x800000;
// 	ADF4001_LE_Clr  ;
	ADF4001_CLK_Clr ;
	for( i=0; i< 24; i++)
	{
		if( data & temp )
			ADF4001_DATA_Set ;
		else 
			ADF4001_DATA_Clr ;
		temp >>= 1;	
		ADF4001_CLK_Set ;
		Delay_1ms(1);
		ADF4001_CLK_Clr ;
		Delay_1ms(1);
	}
	Delay_1ms(1);
	ADF4001_LE_Set ;
	Delay_1ms(1);
 	ADF4001_LE_Clr  ;
}



void TEST_ADF4001()
{
	u16 k = 1000;
	
	ADF4001_Init();	               					// ADF4001初始化	
	
	while(1)
	{
		ADF4001_SetFre(k,LockDetectMode_D);		//数字锁定检测
		k = k+10;
		Delay_ms(1000);
		ADF4001_SetFre(k,LockDetectMode_A);		//模拟锁定检测
		k = k+10;
		Delay_ms(1000);
	}
}

