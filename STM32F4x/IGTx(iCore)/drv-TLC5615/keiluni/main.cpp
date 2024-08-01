// ARMCC-5 GBK @dosconio 20240726
#include "MCU/ST/STM32F4"

using namespace uni;
GPIO_Pin& LEDR = GPIOI[5];

#include "../../../../LibCollection/TLC5615.h"

GPIO_Pin& SCLK = GPIOA[9],
	& DIN = GPIOA[11],
	& DOUT = GPIOF[13],
	& CS = GPIOC[7];


int main() {
	if (!RCC.setClock(SysclkSource::HSE)) while (true);
	LEDR.setMode(GPIOMode::OUT_PushPull);
	TLC5615_t tlc(SCLK,DIN,DOUT,CS);
	byte i = 0;
	while (true) {
		// 5V vDD
		// REF Floating
		LEDR.Toggle();
		tlc.setOutput(100 * i);// 0.5V * i
		SysDelay(3000);
		Ranginc(i, 12);//0~5V
	}
}
