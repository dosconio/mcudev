#include "main.h"

void SysTick_Handler(void) {
	HAL_IncTick();
}

//

void HardFault_Handler(void) {
	while (true);
}


void MemManage_Handler(void) {
	while (true);
}


void BusFault_Handler(void) {
	while (true);
}

void UsageFault_Handler(void) {
	while (true);
}
