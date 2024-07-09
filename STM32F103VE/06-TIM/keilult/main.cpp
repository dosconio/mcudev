// ARMCC-5 GB2312 @dosconio
#include "bsp_basic_tim.h"
#include "stm32f1xx_hal.h"
#include "main.h"

word time = 0;

void TIM_Handler(void) { time++; }
int main() {
	Clock_Initialize();
	LED_Initialize();
	TIM6.setMode();
	TIM6.ConfigMaster();
	TIM6.setInterrupt(TIM_Handler);
	TIM6.setInterruptPriority(1, 0);	
	TIM6.enInterrupt();
	while (true) {
		if ( time == 500 ) // 500 * 1 ms = 0.5 s
		{
			time = 0;
			LED_BLUE_REF().Toggle();
		}  
	}
}
