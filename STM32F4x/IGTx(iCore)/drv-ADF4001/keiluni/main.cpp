// GBK ARMCC6(C11,C++17) TAB4 CRLF
// @dosconio 20240530 ~ {unchecked}

#define _STYLE_RUST
#include "MCU/ST/STM32F4"
#include "datype/uint24"
using namespace uni;

// Pins
GPIO_Pin
	& my_CLK = GPIOA[11],
	& my_LE = GPIOI[3],
	& my_MO = GPIOG[3],
	& my_CE = GPIOA[8],
	& my_DA = GPIOC[7];

#if 1

#define ADF4001LO	300				//300->30Mhz本振，300分频后每次调节100khz
#define LockDetectMode_D 0x1f80C2	//数字锁定检测 
#define LockDetectMode_A 0x1f80D2	//模拟锁定检测（漏极开路输出）

class ADF4001 /*pres(GPIOMode)*/ {
private:
	GPIO_Pin CLK, LE, MO, CE, DA;
	void Send(u24 dat);
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

void ADF4001::Send(u24 dat) {
	u32 inp = dat;
	u32 tmp = 0x1000000;
	CLK = false;
	LE = false;
	for0 (i,24) {
		DA = (inp & (tmp >>= 1));
		CLK = true;
		for0(i,5000){}//delay_ms(1);
		CLK.Toggle();
		for0(i,5000){}//delay_ms(1);
	}
	for0(i,10000){}//delay_ms(1);
	LE = true;
	for0(i,10000){}//delay_ms(1);
	LE.Toggle();
}

ADF4001::ADF4001(const GPIO_Pin& CLK, const GPIO_Pin& LE,\
	const GPIO_Pin& MO, const GPIO_Pin& CE, const GPIO_Pin& DA) :
	CLK(CLK), LE(LE), MO(MO), CE(CE), DA(DA) 
{
	this->CE = true;
	for0(i,5000);//delay_us(5);
	this->CLK = false;
	this->LE = false;//{ISSUE} 
	this->DA = false;
	for0(i,1000);//delay_us(1);
	Send(0x1f8093);// init
	setFrequency(1000, LockDetectMode_D);
}

void ADF4001::setFrequency(u16 F100k, u32 lockmode) {
	Send(0x000000 | ADF4001LO << 2 );	//30Mhz本振，所以是300分频为100Khz基础单位
	Send(0x200001 | F100k << 8 );	//100khz*1000倍频为1Mhz
	Send(lockmode);
}

#endif



GPIO_Pin* POUTS[] = { &my_CLK, &my_LE, &my_CE, &my_DA };
GPIO_Pin* PINNS[] = { &my_MO };

int main() {
	if (!RCC.setClock(SysclkSource::HSE)) while (true);
	GPIO_Pin& LED = GPIOI[5];
	LED.setMode(GPIOMode::OUT_PushPull);
	for0 (i,numsof(POUTS)) POUTS[i]->setMode(GPIOMode::OUT_PushPull);
	for0 (i,numsof(PINNS)) PINNS[i]->setMode(GPIOMode::IN_Pull);

	ADF4001 my_ad(my_CLK,my_LE,my_MO,my_CE,my_DA);
	u16 k = 700;// too little is un-supported
	my_ad.setFrequency(k, LockDetectMode_A);
	for0 (i,1000) {
		my_ad.setFrequency(k,k % 20 ?LockDetectMode_D : LockDetectMode_A);
		k += 10;
		SysDelay(500);
		LED.Toggle(); 
	}
	
	
	while (true);
}
