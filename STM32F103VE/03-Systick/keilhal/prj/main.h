#include <MCU/ST/STM32F103VEx>// Must Define before HAL
#include <stm32f1xx_hal.h>
#include "stm32f1xx_it.h"

#define GPIOB_LED_R 5
#define GPIOB_LED_G 0
#define GPIOB_LED_B 1
#define LED_R (1<<GPIOB_LED_R)//GPIO_PIN_5
#define LED_G (1<<GPIOB_LED_G)
#define LED_B (1<<GPIOB_LED_B)

#define KEY1_PIN          GPIO_PIN_0                 
#define KEY1_GPIO_PORT    GPIOA                      
#define KEY1_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()

#define KEY2_PIN           GPIO_PIN_13                 
#define KEY2_GPIO_PORT    GPIOC                      
#define KEY2_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()

extern "C" uint32 delay_count;

bool Key_down(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

void SystemClock_Config();
