// GBK ARMCC6(C11,C++17) TAB4 CRLF
// @dosconio 20240530 ~ {unchecked}
// learn-from 康威科技
#define _STYLE_RUST
#include "MCU/ST/STM32F407ZEx"
//#include "datype/uint24"
extern "C" {
	#include "delay.h"
}
using namespace uni;

// Pins
GPIO_Pin
	& my_SCK = GPIOB[14],
	& my_SDI = GPIOB[12],
	& my_SEN = GPIOB[14],;

#if 1


class ADF4002 /*pres(GPIOMode)*/ {
private:
	GPIO_Pin CLK, LE, MO, CE, DA;
	void SendWR(u24 dat);
public:
	ADF4001(const GPIO_Pin& CLK, 
		const GPIO_Pin& LE,
		const GPIO_Pin& MO,
		const GPIO_Pin& CE,
		const GPIO_Pin& DA
	);
	void setFrequency(u16 F100k, u32 lockmode);

}; 
void ADF4001_Control(u8 Rdivide, u8 Ndivide);
#endif
#if 1

void ADF4001::SendWR(u24 dat) {
	u32 inp = dat;
	u32 tmp = 0x800000;
	//{ISSUE} LE = false;
	CLK = false;
	for0 (i,24) {
		DA = dat & tmp;
		tmp >>= 1;
		CLK = true;
		delay_ms(1);
		CLK = false;
		delay_ms(1);
	}
	delay_ms(1);
	LE = true;
	delay_ms(1);
	LE = false;
}

ADF4001::ADF4001(const GPIO_Pin& CLK, const GPIO_Pin& LE,\
	const GPIO_Pin& MO, const GPIO_Pin& CE, const GPIO_Pin& DA) :
	CLK(CLK), LE(LE), MO(MO), CE(CE), DA(DA) 
{
	this->CE = true;
	delay_us(5);
	this->LE = false;
	this->CLK = false;
	this->DA = false;
	delay_us(1);
	setFrequency(1000, LockDetectMode_D);
}

void ADF4001::setFrequency(u16 F100k, u32 lockmode) {
	SendWR(0x1f8093);// init
	for0 (i,2) delay006ns(); // delay10ns
	SendWR(0x000000 | ADF4001LO << 2 );	//30Mhz本振，所以是300分频为100Khz基础单位
	for0 (i,2) delay006ns(); // delay10ns
	SendWR(0x200001 | F100k << 8 );	//100khz*1000倍频为1Mhz
	for0 (i,2) delay006ns(); // delay10ns
	SendWR(lockmode);
}

#endif



GPIO_Pin* POUTS[] = { &my_CLK, &my_LE, &my_CE, &my_DA };
GPIO_Pin* PINNS[] = { &my_MO };

int main() {
	if ("GPIO_ADF4001_Configuration") {
		for0 (i,numsof(POUTS)) POUTS[i]->setMode(GPIOMode::OUT_AF_PushPull, GPIOSpeed::Atmost_Medium);
		for0 (i,numsof(PINNS)) PINNS[i]->setMode(GPIOMode::IN_Pull);
	}
	delay_us(10);

	ADF4001 my_ad(my_CLK,my_LE,my_MO,my_CE,my_DA);
	u16 k = 1000;
	
	for0 (i,1000) {
		my_ad.setFrequency(k,k % 20 ?LockDetectMode_D : LockDetectMode_A);
		k += 10;
		delay_ms(1000);
	}
	
	
	while (true);
}
