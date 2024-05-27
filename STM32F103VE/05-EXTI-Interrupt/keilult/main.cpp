// ARMCC-5 GB2312 @dosconio
#include "main.h"
#include "Device/SysTick"
#include "interrupt"

GPIO_Pin
	& RLED = LED_RED_REF(),
	& GLED = LED_GREEN_REF(),
	& BLED = LED_BLUE_REF();

int main() {
	SystemClock_Config();
	LED_Initialize();
	KEY_Initialize();
	
	GPIOA[0].setMode(GPIORupt::Posedge);
	
	
	while(1);
	{ //{TODO} EXTI Mode Configuration for 2 keys
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.Pin = GPIO_PIN_0;
		GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;// INP
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		HAL_GPIO_Init((GPIO_TypeDef *)uni::GPIOA.base, &GPIO_InitStructure); 
		
		HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(EXTI0_IRQn);

		GPIO_InitStructure.Pin = GPIO_PIN_13;  
		HAL_GPIO_Init((GPIO_TypeDef *)uni::GPIOC.base, &GPIO_InitStructure);      
		HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	}
	

	while (true);
}

void SystemClock_Config(void)
{
	if (!(RCC.HSE.setMode() && RCC.PLL.setMode()))// PLL<<HSE*9/1
		while (true);
	if (RCC.setFlash() && RCC.AHB.setMode() && RCC.Sysclock.setMode() &&
		RCC.setFlash(true) && RCC.APB1.setMode(1) && RCC.APB2.setMode(0)   );
	else while (true);
	// Update the SystemCoreClock global variable
	SystemCoreClock = HAL_RCC_GetSysClockFreq() >> //{TODO}
		AHBPrescTable[(RCC.Cfgreg & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos];
	SysTick::enClock(SystemCoreClock, 1000);// T = 1ms //{TODO} Auto get `SystemCoreClock`
}

#include <cinc>
volatile stduint delay_count = 0;
void SysTick_Handler(void) {
	delay_count && delay_count--;
}
void SysDelay(stduint ms) {
	delay_count = ms; 
	while(delay_count);// must add `volatile`
}
/*
void EXTI0_IRQHandler(void) {
	// KEY1 GPIOA0
	if (EXTI->PR & (GPIO_PIN_0))//if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET) 
	{
		RLED.Toggle();
		EXTI->PR = GPIO_PIN_0; //__HAL_GPIO_EXTI_CLEAR_IT
	}
}

void EXTI15_10_IRQHandler(void) {
	// KEY2 GPIOC13
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET) 
	{
		GLED.Toggle();
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);     
	}
}*/
#include <cinc>
