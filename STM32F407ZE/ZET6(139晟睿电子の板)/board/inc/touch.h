#ifndef __TOUCH_H__
#define __TOUCH_H__

#include <stm32f4xx.h>
	 
#define TCS_LOW    	GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define TCS_HIGH   	GPIO_SetBits(GPIOB, GPIO_Pin_12)     
#define PEN_int		(GPIOG->IDR&GPIO_Pin_7)

#define CMD_RDY 0X90  
#define CMD_RDX	0XD0    											 	    
			  
void Touch_Init(void);		 		 
u16 ADS_Read(u8 CMD);	 
u16 Read_XY(u8 CMD);
	 
#endif


