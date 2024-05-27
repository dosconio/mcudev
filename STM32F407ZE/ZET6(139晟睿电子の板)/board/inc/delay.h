#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx_conf.h"
#include "integer/ruststyle.h"

void delay_ms(u16 nms);
void delay_us(u32 nus);
void Delay(u32 count);

#endif

