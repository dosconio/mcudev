// ARMCC-5 GB2312 @dosconio
#include "main.h"

void int_key1() { RLED.Toggle(); }
void int_key2() { GLED.Toggle(); }

int main() {
	Clock_Initialize();
	LED_Initialize();
	KEY_Initialize(); {
		KEY_1_REF().setMode(GPIORupt::Posedge);
		KEY_1_REF().setInterrupt(int_key1);
		KEY_1_REF().setInterruptPriority(0,0); // Optional
		KEY_2_REF().setMode(GPIORupt::Posedge);
		KEY_2_REF().setInterrupt(int_key2);
		KEY_2_REF().setInterruptPriority(0,0); // Optional
	}
	
	KEY_1_REF().enInterrupt();
	KEY_2_REF().enInterrupt();
	
	while (true) {
		BLED.Toggle();
		SysDelay(1000);// 1s
	}
}
