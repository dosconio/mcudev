#ifndef _MQ135_INC_
#define _MQ135_INC_

#include "MCU/ST/STM32F103VEx"

// 空气质量传感器です！

extern "C" uint16_t ADC_ConvertedValue[8];
 
void  MQ135_Initialize(void);

uint16 MQ135_GetAnalog();
	
#endif
