// ARMCC-5 GB2312 @dosconio
#include <stm32f1xx_hal.h>//{TODO} independent from HAL
#include "MCU/ST/STM32F103VEx"
#include "Device/SysTick"
#include "./led.h"

using namespace uni;

extern "C" void SystemClock_Config(void);

int main() {
	LED_Initialize();
	SystemClock_Config();

	GPIO_Pin& RLED = LED_RED_REF(),
		& GLED = LED_GREEN_REF(),
		& BLED = LED_BLUE_REF();
	while (true) {
		RLED.Toggle();
		HAL_Delay(333);
		RLED.Toggle();
		GLED.Toggle();
		HAL_Delay(333);
		GLED.Toggle();
		BLED.Toggle();
		HAL_Delay(333);
		BLED.Toggle();
	}
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