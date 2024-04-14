#include <MCU/ST/STM32F103VEx>// Must Define before HAL
#include <stm32f1xx_hal.h>
#include "stm32f1xx_it.h"

#define GPIOB_LED_R 5
#define GPIOB_LED_G 0
#define GPIOB_LED_B 1
#define LED_R (1<<GPIOB_LED_R)//GPIO_PIN_5
#define LED_G (1<<GPIOB_LED_G)
#define LED_B (1<<GPIOB_LED_B)

void SystemClock_Config();
