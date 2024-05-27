#include "main.h"


uint32 delay_count = 0;
void SysTick_Handler(void) {
	HAL_IncTick();
	delay_count && delay_count--;
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
