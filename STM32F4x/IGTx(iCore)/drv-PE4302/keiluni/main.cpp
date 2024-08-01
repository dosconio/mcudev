// ARMCC-5 GB2312 @dosconio

#include "MCU/ST/STM32F4"

using namespace uni;

#include "../../../../LibCollection/PE4302.h"

void my_delay(void) { for0(i, 10000); }
GPIO_Pin &SDA = GPIOA[6];
GPIO_Pin &CLK = GPIOC[2];
GPIO_Pin &LE1 = GPIOC[3];
GPIO_Pin &LE2 = GPIOF[9];

int main() {
	RCC.setClock(SysclkSource::HSE);
	GPIO_Pin& LEDR = GPIOI[5];
	LEDR.setMode(GPIOMode::OUT_PushPull);
	HMC624A_t hmc(SDA, CLK, LE1);
	HMC624A_t hm2(SDA, CLK, LE2);
	word i = 0;

	while (true)
	{
		SysDelay(2000);
		LE1 = LE2 = 0;
		hmc.setOutput(0);
		LE1 = LE2 = 0;
		hm2.setOutput(0);
		LE1 = LE2 = 0;
		LEDR.Toggle();
		i %= 0x40;
	}
}

