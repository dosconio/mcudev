#include <stm32f1xx_hal.h>//{TODO} independent from HAL
#include "./led.h"
#include "./key.h"
#include "Device/IIC"
#include "driver/Radio/RDA5820.h"

using namespace uni;

void SystemClock_Config(void);
