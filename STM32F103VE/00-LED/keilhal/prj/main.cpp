// ARMCC-5 GB2312 @dosconio

#include <MCU/ST/STM32F103VEx>// Must Define before HAL
#include <stm32f1xx_hal.h>

#define GPIOB_LED_R 5
#define GPIOB_LED_G 0
#define GPIOB_LED_B 1
#define LED_R (1<<GPIOB_LED_R)//GPIO_PIN_5
#define LED_G (1<<GPIOB_LED_G)
#define LED_B (1<<GPIOB_LED_B)

int main() {
	HAL_Init();
	RCC->AHBENR |= 1<<7;// ���� GPIOH ʱ�ӣ�ʹ������ʱ��Ҫ�ȿ�������ʱ��
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef g;
	g.Pin = LED_R | LED_B;
	g.Mode = GPIO_MODE_OUTPUT_PP;
	g.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &g);
	HAL_GPIO_WritePin(GPIOB, LED_R|LED_B, GPIO_PIN_RESET);
	while (true);
}
