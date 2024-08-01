// ARMCC-5 GB2312 @dosconio

#include "MCU/ST/STM32F4"

using namespace uni;

typedef GeneralPurposeInputOutputPin sbit;

#include "../../../../LibCollection/HMC624A.h"

void my_delay(void) { for0(i, 1000); }
sbit &SDA = GPIOI[9];
sbit &CLK = GPIOE[1];
sbit &LEs = GPIOF[2];

int main() {
	RCC.setClock(SysclkSource::HSE);
	GPIO_Pin& LEDR = GPIOI[5];
	LEDR.setMode(GPIOMode::OUT_PushPull);
	HMC624A_t hmc(SDA, CLK, LEs);

	while (true)
	{
		SysDelay(1000);
		hmc.setOutput(2);
		LEDR.Toggle();
		// EXPI 20240801 0705:
		// The debuff rate seems to be related with Freq of the signal
		// EXPI 20240801 0147:
		// 2Vpp -> 0x02:180mV 0x1F:500mV
		// EXPI 20240801 Table for 40M Sine
		// dB 0     2     4     6     8     10    12    14    16    18    20
		// de 1.000,0.794,0.631,0.501,0.398,0.316,0.251,0.200,0.158,0.126,0.100
		// ky x     x     0x3E ,56   ,46   ,42   ,37   ,28   ,14   ,10   ,5
	}
}

