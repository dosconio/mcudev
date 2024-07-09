// ARMCC-5 GB2312 @dosconio
#include "main.h"
#include "Device/SysTick"

int main() {
	SystemClock_Config();

	LED_Initialize();
	KEY_Initialize();
	
	GPIO_Pin& RLED = LED_RED_REF(),
		& GLED = LED_GREEN_REF(),
		& BLED = LED_BLUE_REF();
	RLED = false;
	while (true) {
		if (KEY_Scanwait(KEY_1_REF()))
			RLED.Toggle();
		if (KEY_Scanwait(KEY_2_REF()))
			GLED.Toggle();
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

