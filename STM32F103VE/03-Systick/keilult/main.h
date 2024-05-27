#include <stm32f1xx_hal.h>//{TODO} independent from HAL
#include "./led.h"
#include "./key.h"

using namespace uni;

extern "C" {
	void SysDelay(stduint ms);
}

void SystemClock_Config(void);
