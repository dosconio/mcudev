// ARMCC-5 GBK @dosconio
#include "MCU/ST/STM32F407ZEx"
#include "driver/Video/ILI9320.h"
using namespace uni;
GPIO_Pin& LEDR = GPIOI[5];

void my_delay(stduint i) {
	for0(_i, i);
}

int main() {
	LEDR.setMode(GPIOMode::OUT_PushPull);
	if (!RCC.setClock(SysclkSource::HSE)) while (true);
	SysTick::enClock(1000000);
	
	GPIO_Pin* Dats[16] = {
		&GPIOB[ 9], &GPIOH[ 7], &GPIOH[14], &GPIOH[ 6],
		&GPIOH[13], &GPIOG[ 1], &GPIOH[12], &GPIOG[ 0],
		&GPIOH[11], &GPIOF[15], &GPIOH[10], &GPIOF[14], 
		&GPIOH[ 9], &GPIOF[11], &GPIOH[ 8], &GPIOF[12] 
	};
	GPIO_Pin& CS = GPIOI[3],
		&RS = GPIOI[1],
		&WR = GPIOA[11],
		&RD = GPIOA[9],
		&RST = GPIOC[7];
	ILI9320_FreePins my_ili(16, Dats, WR, RD, RS, RST, CS, CS, CS);
	my_ili.func_delay_us = my_delay; // SysDelay;
	VideoControlBlock LCD(0, &my_ili);
	my_ili.setMode(16);
	my_ili.Clear();
	uint32 ii = 10;
	while (true) {
		my_ili.Clear(ii);
		ii+=100;
		ii%= 0xFF00;
		LEDR.Toggle();
		SysDelay(250*1000*4);// ms
	}

	while (1);
}
