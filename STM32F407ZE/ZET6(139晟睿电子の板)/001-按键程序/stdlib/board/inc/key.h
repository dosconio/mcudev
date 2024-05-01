#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx_conf.h"

typedef enum 
{  
  KEY1 = 0,
  KEY2 = 1,
  KEY3 = 2,
  KEY4 = 3,
} Key_Def;

static GPIO_TypeDef* KEY_PORT[4]={GPIOA, GPIOA, GPIOC, GPIOD};
static const u16 KEY_PIN[4]={GPIO_Pin_0, GPIO_Pin_8, GPIO_Pin_13,GPIO_Pin_3};

void Key_Init(void);
u8 Get_Key(Key_Def key);
u8 KEY_Scan(void);

#endif

