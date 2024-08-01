// ARMCC-5 GB2312 @dosconio
// AD9951 and 9952 and 9954 (SPI)
#include "MCU/ST/STM32F4"

using namespace uni;

typedef GeneralPurposeInputOutputPin sbit;

#include "../../../../LibCollection/AD995x.h"

sbit &PWRDWNCTL = GPIOB[9];
sbit &RST = GPIOF[0];
sbit &IOSYNC  = GPIOB[6];
sbit &SDO = GPIOG[9];
sbit &CS = GPIOF[10];
sbit &SCLK = GPIOC[3];
sbit &SDIO = GPIOH[3];
sbit &OSK = GPIOA[6];
sbit &UPDATA = GPIOH[2];

int main() {
	RCC.setClock(SysclkSource::HSE);
	GPIO_Pin& LEDR = GPIOI[5];
	LEDR.setMode(GPIOMode::OUT_PushPull);

	AD9954 my_ad(PWRDWNCTL, RST, IOSYNC, SDO, CS, SCLK, SDIO, OSK, UPDATA);
	my_ad.setMode();
	my_ad.setFrequency(100.0e0);
	while (true)
	{
		my_ad.setPhase(0);
		SysDelay(50);
		my_ad.setPhase(0x4000 >> 1);
		SysDelay(50);
	}
}

