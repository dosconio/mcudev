#ifndef _MQ135_INC_
#define _MQ135_INC_

#include "MCU/ST/STM32F103VEx"

// ���������������Ǥ���

extern "C" uint16_t ADC_ConvertedValue[8];
 
void  MQ135_Initialize(void);

uint16 MQ135_GetAnalog();
	
#endif
