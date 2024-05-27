#include "main.h"


int main(void)
{
	Key_Init();
	LED_Init();
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	while(1)
	{
		delay_ms(500);
		if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)); else 
			GPIO_WriteBit(GPIOB, GPIO_Pin_0, !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0));
	}
}


