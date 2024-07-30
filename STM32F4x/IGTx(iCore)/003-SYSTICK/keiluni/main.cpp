// ARMCC-5 GBK @dosconio
#include "MCU/ST/STM32F4"
using namespace uni;
GPIO_Pin& LEDR = GPIOI[5];

int main() {
	LEDR.setMode(GPIOMode::OUT_PushPull);
	if (!RCC.setClock(SysclkSource::HSE)) while (true);
	
	while (true) {
		LEDR.Toggle();
		SysDelay(250);// ms
	}
}
