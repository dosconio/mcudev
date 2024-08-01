// ARMCC-5 GBK @dosconio 20240726
#include "MCU/ST/STM32F4"

using namespace uni;
GPIO_Pin& LEDR = GPIOI[5];

#include "../../../../LibCollection/MCP4725.h"

int main() {
	if (!RCC.setClock(SysclkSource::HSE)) while (true);
	LEDR.setMode(GPIOMode::OUT_PushPull);
	MCP4725_t my_mcp(GPIOI[1], GPIOI[3]);
	while (true) {
		LEDR.Toggle();
		SysDelay(2000);// ms
		my_mcp.setOutput(0xFFF);
		LEDR.Toggle();
		SysDelay(2000);// ms
		my_mcp.setOutput(0x3FF);
	}
}
