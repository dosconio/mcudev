// ARMCC-5 GB2312 @dosconio
// make use of KDT9959测试参考程序-STM32F103X-相位优化版本-V3.0
#include "MCU/ST/STM32F4"

using namespace uni;

typedef GeneralPurposeInputOutputPin sbit;

#include "../../../../LibCollection/AD995x.h"

void my_delay(void) { for0(i, 1000); }

sbit &RST = GPIOB[9];
sbit &IOUP = GPIOF[0];
sbit &SDIO  = GPIOB[6];
sbit &SCLK = GPIOG[9];
sbit &CSB = GPIOF[10];

int main() {
	RCC.setClock(SysclkSource::HSE);
	GPIO_Pin& LEDR = GPIOI[5];
	LEDR.setMode(GPIOMode::OUT_PushPull);
	AD9959 my_ad(RST, IOUP, SDIO, SCLK, CSB, my_delay);
	my_ad.setMode();
	// chan0 23mV 1MHz 000fz-dg
	my_ad.setAmplitude(0, 50);
	my_ad.setFrequency(0, 1e6);
	my_ad.setPhase(0, 0);
	// chan1 46mV 2MHz 090fz-dg
	my_ad.setAmplitude(1, 100);
	my_ad.setFrequency(1, 2e6);
	my_ad.setPhase(1, 90);
	// chan2 92mV 4MHz 180fz-dg
	my_ad.setAmplitude(2, 200);
	my_ad.setFrequency(2, 4e6);
	my_ad.setPhase(2, 180);
	// chan3 137mV 8MHz 045fz-dg
	my_ad.setAmplitude(3, 300);
	my_ad.setFrequency(3, 8e6);
	my_ad.setPhase(3, 45);	

	while (true)
	{
		LEDR.Toggle();
		SysDelay(500);
	}
}

/*
[PHASE]
	ad1.setAmplitude(0, 50);
	ad1.setFrequency(0, 1e6);
	ad1.setPhase(0, 0);		
	ad1.setAmplitude(1, 50);
	ad1.setFrequency(1, 1e6);
	ad1.setPhase(1, 0);	
	while (true) {
		static stduint adpz = 0;
		Ranginc(adpz, 360);
		ad1.setPhase(1, adpz);	
		SysDelay(10);
	}
*/
