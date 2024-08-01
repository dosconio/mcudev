// ARMCC-5 GB2312 @dosconio
#include "MCU/ST/STM32F4"

using namespace uni;
#define GPIOB_LED 0

GPIO_Pin& LED = GPIOB[0];
GPIO_Pin& KEY = GPIOA[0];


void hand(void) {
	LED.Toggle();
}

int main() {
	RCC.setClock(SysclkSource::HSE);
	LED.setMode(GPIOMode::OUT_PushPull);
	KEY.setMode(GPIORupt::Negedge);
	KEY.setInterrupt(hand);
	KEY.enInterrupt();

	while (true) {
		SysDelay(250);
	}
}
