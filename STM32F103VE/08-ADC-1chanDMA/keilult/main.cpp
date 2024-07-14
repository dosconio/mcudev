// ARMCC-5 GB2312 @dosconio
#include "stm32f1xx_hal.h"
extern "C" {
#include "bsp_debug_usart.h"
	#include "stdio.h"
}
#include "main.h"

word time = 0;
dword adc_res = 0;
word dma_buf[4];
char buf[33];
void ADC_Handler(void) { 
	ADC1 >> adc_res;
}
int main() {
	Clock_Initialize();
	LED_Initialize();
	GPIOC[1].setMode(GPIOMode::IN_Analog);
	ADC1.setDMA(false, true /*timebase array*/);// ADC1.setDMA() for only a value but an array
	ADC1.setMode(0, false, 0, false, 8); // 72M / 8 = 9MHz, 55.5 cycles + 12.5 = 68 cycles,
	ADC1.setChannel(GPIOC[1]);
	ADC1.enDMA(dma_buf, numsof(dma_buf) - 1 /*debug len*/);// run this multi-times for once mode
	
	DEBUG_USART_Config(); Usart_SendString("@dosconio\r\n");
	while (true) {
		SysDelay(500);
		sprintf(buf, "%04X %04X %04X %04X\r\n", dma_buf[0], dma_buf[1], dma_buf[2], dma_buf[3]);
		Usart_SendString(buf); 
		LED_BLUE_REF().Toggle();
	}
}
