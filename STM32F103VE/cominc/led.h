#ifndef _LED_INC_
#define _LED_INC_

#include "MCU/ST/STM32F103VEx"

#define GPIOB_LED_R 5
#define GPIOB_LED_G 0
#define GPIOB_LED_B 1
#define LED_R (1<<GPIOB_LED_R)//GPIO_PIN_5
#define LED_G (1<<GPIOB_LED_G)
#define LED_B (1<<GPIOB_LED_B)

void LED_Light(bool red, bool green, bool blue);// need not init

void LED_Initialize();

uni::GPIO_Pin& LED_RED_REF();

uni::GPIO_Pin& LED_GREEN_REF();

uni::GPIO_Pin& LED_BLUE_REF();

#endif
