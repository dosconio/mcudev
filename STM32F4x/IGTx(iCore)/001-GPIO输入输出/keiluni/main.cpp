// ARMCC-5 GB2312 @dosconio
#include "MCU/ST/STM32F4"
using namespace uni;
GPIO_Pin& LEDR = GPIOI[5];
GPIO_Pin& LEDG = GPIOI[6];
GPIO_Pin& LEDB = GPIOI[7];
GPIO_Pin& KEY = GPIOH[15];

int main() {
	LEDR.setMode(GPIOMode::OUT_PushPull);
	LEDG.setMode(GPIOMode::OUT_PushPull);
	LEDB.setMode(GPIOMode::OUT_PushPull);
	KEY.setMode(GPIOMode::IN_Floating);
	while (true) {
		LEDB = KEY;
	}
	while (1);
}
