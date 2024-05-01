#include "led.h"

void LED_Init(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure;

  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;         //PB0
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

}
void LEDOn(Led_Def Led)//LED��
{
  	LED_PORT[Led]->BSRRH=LED_PIN[Led];
}

void LEDOff(Led_Def Led)//LED��
{
  	LED_PORT[Led]->BSRRL=LED_PIN[Led];  
}

void LEDTog(Led_Def Led)//LED״̬��ת
{
  	LED_PORT[Led]->ODR^=LED_PIN[Led];
}



