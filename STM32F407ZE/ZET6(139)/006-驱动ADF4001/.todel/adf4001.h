/*--------------------------------------------------------------
头文件：ADF4001.h				  
---------------------------------------------------------------*/
#ifndef _ADF4001_H_
#define _ADF4001_H_

#include <stm32f10x.h>

//----------------------------------------------------------------
// ADF4001_I/O控制线\
//-----------------------------------------------------------------

//#define ADF4001_CE_Set    (GPIO_SetBits(GPIOC,GPIO_Pin_0))         
//#define ADF4001_CE_Clr    (GPIO_ResetBits(GPIOC,GPIO_Pin_0))
//#define ADF4001_CLK_Set   (GPIO_SetBits(GPIOC,GPIO_Pin_1))				   
//#define ADF4001_CLK_Clr   (GPIO_ResetBits(GPIOC,GPIO_Pin_1))
//#define ADF4001_DATA_Set  (GPIO_SetBits(GPIOC,GPIO_Pin_2))           
//#define ADF4001_DATA_Clr  (GPIO_ResetBits(GPIOC,GPIO_Pin_2))
//#define ADF4001_LE_Set    (GPIO_SetBits(GPIOC,GPIO_Pin_3))         
//#define ADF4001_LE_Clr    (GPIO_ResetBits(GPIOC,GPIO_Pin_3))

#if defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined (STM32F10X_LD) || defined  (STM32F10X_XL)
#define GPIO_ADF4001_Pin_CE			GPIO_Pin_11 					//
#define GPIO_ADF4001_Port_CE		GPIOC
#define RCC_ADF4001_CE				RCC_APB2Periph_GPIOC
#define GPIO_ADF4001_Pin_LE 		GPIO_Pin_10					//
#define GPIO_ADF4001_Port_LE		GPIOC
#define RCC_ADF4001_LE				RCC_APB2Periph_GPIOC
#define GPIO_ADF4001_Pin_CLK 		GPIO_Pin_12 					//
#define GPIO_ADF4001_Port_CLK 		GPIOC
#define RCC_ADF4001_CLK 			RCC_APB2Periph_GPIOC
#define GPIO_ADF4001_Pin_DATA 		GPIO_Pin_9					//
#define GPIO_ADF4001_Port_DATA		GPIOC
#define RCC_ADF4001_DATA			RCC_APB2Periph_GPIOC

#define GPIO_ADF4001_Pin_MOUT 		GPIO_Pin_0					//
#define GPIO_ADF4001_Port_MOUT		GPIOE
#define RCC_ADF4001_MOUT			RCC_APB2Periph_GPIOE

#endif


#define ADF4001_CE_Set    (GPIO_SetBits(GPIO_ADF4001_Port_CE,GPIO_ADF4001_Pin_CE))         
#define ADF4001_CE_Clr    (GPIO_ResetBits(GPIO_ADF4001_Port_CE,GPIO_ADF4001_Pin_CE))
#define ADF4001_CLK_Set   (GPIO_SetBits(GPIO_ADF4001_Port_CLK,GPIO_ADF4001_Pin_CLK))				   
#define ADF4001_CLK_Clr   (GPIO_ResetBits(GPIO_ADF4001_Port_CLK,GPIO_ADF4001_Pin_CLK))
#define ADF4001_DATA_Set  (GPIO_SetBits(GPIO_ADF4001_Port_DATA,GPIO_ADF4001_Pin_DATA))           
#define ADF4001_DATA_Clr  (GPIO_ResetBits(GPIO_ADF4001_Port_DATA,GPIO_ADF4001_Pin_DATA))
#define ADF4001_LE_Set    (GPIO_SetBits(GPIO_ADF4001_Port_LE,GPIO_ADF4001_Pin_LE))         
#define ADF4001_LE_Clr    (GPIO_ResetBits(GPIO_ADF4001_Port_LE,GPIO_ADF4001_Pin_LE))

#define ADF4001_MOUT_READ()	GPIO_ReadInputDataBit(GPIO_ADF4001_Port_MOUT,GPIO_ADF4001_Pin_MOUT);



#define ADF4001LO	300				//300->30Mhz本振，300分频后每次调节100khz
#define LockDetectMode_D 0x1f80C2	//数字锁定检测 
#define LockDetectMode_A 0x1f80D2	//模拟锁定检测（漏极开路输出）

//**********************************************************
void GPIO_ADF4001_Configuration(void);						// ADF4001_I/O配置
void ADF4001_WR_Data(unsigned long int data);  
void ADF4001_Init(void);	
void ADF4001_Control(u8 Rdivide,u8 Ndivide);
void ADF4001_SetFre(u16 F100k,u32 lockmode);

void TEST_ADF4001(void);

#endif
