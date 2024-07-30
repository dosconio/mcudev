// ARMCC-5 GBK @dosconio
#include "MCU/ST/STM32F4"
using namespace uni;
GPIO_Pin& LEDR = GPIOI[5];

int main() {
	// 在system_stm32f4xx.c的SystemInit()函数把系统时钟初始化成16MHZ
	// 系统时钟标准设置为168M，最高是216M，不推荐
	// ｛暂不支持 HSI｝
	// ｛暂不使用 SysTick 延时｝
	LEDR.setMode(GPIOMode::OUT_PushPull);
	for0 (i, 3*2) {
		LEDR.Toggle();
		for0(_i, 1000000);
	}
	if (!RCC.setClock(SysclkSource::HSE)) while (true);
	
	while (true) {
		LEDR.Toggle();
		for0(_i, 1000000);
	}
}
