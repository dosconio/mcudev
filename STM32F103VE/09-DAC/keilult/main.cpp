// ARMCC-5 GB2312 @dosconio
#include "stm32f1xx_hal.h"
extern "C" {
	#include "stdio.h"
}
#include "main.h"

word buf[128];

int main() {
	Clock_Initialize();
	DAC.enDMA(GPIOA[5], DACTrigger::T6, false, buf, numsof(buf));
	for0a(i, buf) {
		if (i < 64) buf[i] = 0x1000 / 64 * i;
		else buf[i] = 0x1000 - 0x1000 / 64 * (i-64+1);
	}
	while (true);
}
