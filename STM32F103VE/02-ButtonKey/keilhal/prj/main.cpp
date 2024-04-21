// ARMCC-5 GB2312 @dosconio

#include "main.h"

int main() {
	HAL_Init();
	SystemClock_Config();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef g;
	// for LED
	g.Pin = LED_R | LED_B;
	g.Mode = GPIO_MODE_OUTPUT_PP;
	g.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &g);
	// for KEY
	g.Mode = GPIO_MODE_INPUT;
	g.Pull = GPIO_NOPULL;
	g.Pin = KEY1_PIN;
	HAL_GPIO_Init(KEY1_GPIO_PORT, &g);
	g.Pin = KEY2_PIN;
	HAL_GPIO_Init(KEY2_GPIO_PORT, &g);
	
	KEY1_ENABLE();
	KEY2_ENABLE();
	
	HAL_GPIO_WritePin(GPIOB, LED_R | LED_B, GPIO_PIN_SET);
	while (true) {
		if(Key_down(KEY1_GPIO_PORT,KEY1_PIN))
			HAL_GPIO_TogglePin(GPIOB, LED_R);
		if(Key_down(KEY2_GPIO_PORT,KEY2_PIN))
			HAL_GPIO_TogglePin(GPIOB, LED_B);
	}
}

bool Key_down(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin) {			
	const bool NegativeLogic = false;
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) ^ NegativeLogic) {	 
		while(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) ^ NegativeLogic);// stuck method  
		return 	true ^ NegativeLogic;	// GDP
	}
	else return false ^ NegativeLogic;
}

void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef clkinitstruct = {0};
	RCC_OscInitTypeDef oscinitstruct = {0};

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
	oscinitstruct.HSEState        = RCC_HSE_ON;
	oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
	oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
	oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
	oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
	{
		/* Initialization Error */
		while(1); 
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
	 clocks dividers */
	clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
	clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
	if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
	{
		/* Initialization Error */
		while(1); 
	}
}
