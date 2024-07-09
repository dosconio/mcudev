

extern "C" {
#include "stm32f1xx_hal.h"
}
#include "../cominc/system.h"
using namespace uni;

extern "C" {
	void SystemClock_Config(void);
}

void SystemClock_Config(void)
{
	//{TODO} SystemCoreClock RCC::Initialize(void)
	if (!(RCC.HSE.setMode() && RCC.PLL.setMode()))// PLL<<HSE*9/1
		while (true);
	if (RCC.setFlash() && RCC.AHB.setMode() && RCC.Sysclock.setMode() &&
		RCC.setFlash(true) && RCC.APB1.setMode(1) && RCC.APB2.setMode(0)   );
	else while (true);
	// Update the SystemCoreClock global variable
	SystemCoreClock = HAL_RCC_GetSysClockFreq() >> //{TODO}
		AHBPrescTable[(RCC.Cfgreg & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos];
	
}

void Clock_Initialize() { 
	SystemClock_Config(); 
	SysTick::enClock(SystemCoreClock, 1000);// T = 1ms //{TODO} Auto get `SystemCoreClock`
}
