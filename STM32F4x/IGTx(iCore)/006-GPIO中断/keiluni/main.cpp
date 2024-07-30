// ARMCC-5 GBK @dosconio
#include "MCU/ST/STM32F4"
using namespace uni;
GPIO_Pin& LEDR = GPIOI[5];
GPIO_Pin& LEDG = GPIOI[6];
GPIO_Pin& LEDB = GPIOI[7];
GPIO_Pin& KEY = GPIOH[15];
void my_handler() {
	LEDR.Toggle();
}

int main() {
	if (!RCC.setClock(SysclkSource::HSE)) while (true);
	LEDR.setMode(GPIOMode::OUT_PushPull);
	KEY.setMode(GPIORupt::Anyedge);
	KEY.setInterrupt(my_handler);
	KEY.enInterrupt();
	while (true);
}