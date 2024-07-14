#ifndef _MAIN
#define _MAIN

#include <stm32f1xx_hal.h>//{TODO} independent from HAL
#include "./led.h"
#include "./key.h"
#include "./lcd.h"
#include "./rtc.h"

extern "C" {
	void SysDelay(stduint ms);
}

void SystemClock_Config(void);

#endif
