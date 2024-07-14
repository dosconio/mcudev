// ARMCC-5 GB2312 @dosconio
#include "stm32f1xx_hal.h"
extern "C" {
#include "bsp_debug_usart.h"
	#include "stdio.h"
}
#include "main.h"

word time = 0;
dword adc_res = 0;
char buf[33];
void ADC_Handler(void) { 
	ADC1 >> adc_res;
}
int main() {
	Clock_Initialize();
	LED_Initialize();
	GPIOC[1].setMode(GPIOMode::IN_Analog);
	
	ADC1.setMode();
	ADC1.setChannel(GPIOC[1]);
	ADC1.setInterrupt(ADC_Handler);
	ADC1.setInterruptPriority(0, 0);// OPT
	ADC1.enInterrupt();
	
	DEBUG_USART_Config(); Usart_SendString("@dosconio\r\n");
	while (true) {
		SysDelay(500);
		sprintf(buf, "0x%04X \r\n", adc_res);
		Usart_SendString(buf); 
		LED_BLUE_REF().Toggle();
	}
}
