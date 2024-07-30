// ARMCC-5 GB2312 @dosconio
#include "MCU/ST/STM32F4"
using namespace uni;
GPIO_Pin& LEDR = GPIOI[5];
GPIO_Pin& LEDG = GPIOI[6];
GPIO_Pin& LEDB = GPIOI[7];

int main() {
	LEDR.setMode(GPIOMode::OUT_PushPull);
	LEDG.setMode(GPIOMode::OUT_PushPull);
	LEDB.setMode(GPIOMode::OUT_PushPull);
	stduint i = 0;
	while (true) {
		LEDR = !BitGet(i, 0);
		LEDG = !BitGet(i, 1);
		LEDB = !BitGet(i, 2);
		for0(_i, 1000000);
		Ranginc(i, 0x8);
	}
}
