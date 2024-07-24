// ARMCC-5 GBK @dosconio
#include "MCU/ST/STM32F407ZEx"
using namespace uni;
GPIO_Pin& LEDR = GPIOI[5];

int main() {
	LEDR.setMode(GPIOMode::OUT_PushPull);
	// GPIOB[2].setMode(GPIOMode::OUT_PushPull);
	if (!RCC.setClock(SysclkSource::HSE)) while (true);// Ĭ�� 168M 1000Hz-SysTick���ݲ�֧�� HSI��
	
	while (true) {
		LEDR.Toggle();
		// GPIOB[2].Toggle();
		SysDelay(250);// ms
	}
	while (1);
}
