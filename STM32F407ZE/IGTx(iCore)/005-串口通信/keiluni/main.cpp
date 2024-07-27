// ARMCC-5 GBK @dosconio
#include "MCU/ST/STM32F407ZEx"
using namespace uni;
GPIO_Pin& LEDR = GPIOI[5];

void my_uart() {
	int data;
	XART1 >> data;
	XART1 << data + 1;
}

int main() {
	LEDR.setMode(GPIOMode::OUT_PushPull);
	if (!RCC.setClock(SysclkSource::HSE)) while (true);
	XART1.setMode();
	XART1.setInterruptPriority(0,1);
	XART1.setInterrupt(my_uart);
	XART1.enInterrupt();
	XART1 << "Hello~\n";
	while (true) {
		LEDR.Toggle();
		XART1 << ">>>\n";
		SysDelay(1000);
	}
	
	while (1);
}