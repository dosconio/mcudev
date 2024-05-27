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
	// MCO
	GeneralPurposeInputOutputPin& MCO = GPIO['A'][8];
	MCO.setMode(GPIOMode::OUT_AF_PushPull, GPIOSpeed::Atmost_10MHz);
	GPIO['A'].enClock();
	
	while (true) {
		RLED.Toggle();
		SysDelay(500);
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
	
	// MCO，输出为PLLCLK时钟2分频:36MHz
	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_1); 
	
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
#include <cinc>
