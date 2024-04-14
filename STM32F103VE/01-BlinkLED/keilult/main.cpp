// ARMCC-5 GB2312 @dosconio

#include "./led.h"

using namespace uni;

//{TODO} independent from HAL
#include <stm32f1xx_hal.h>
extern "C" void SystemClock_Config(void);
extern void depend();

int main() {
	depend();
	LED_Initialize();
	
	GPIO_Pin& RLED = LED_RED_REF();
	GPIO_Pin& GLED = LED_GREEN_REF();
	GPIO_Pin& BLED = LED_BLUE_REF();
	
	while (true) {
		RLED.Toggle();
		HAL_Delay(250);
	}
}


void depend() {
	HAL_Init();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	SystemClock_Config();
}

#include <cinc>

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

void SysTick_Handler(void)
{
	HAL_IncTick();
}

#include <cinc>
