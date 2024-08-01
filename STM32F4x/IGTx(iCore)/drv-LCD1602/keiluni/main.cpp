// ARMCC-5 GBK @dosconio 20240726
//｛TEMP｝ 注意中断中使用的情况，现在可能会产生竞争冲突导致乱码！
#include "MCU/ST/STM32F4"
#include "driver/LCD1602.h"
using namespace uni;
GPIO_Pin& LEDR = GPIOI[5];

int main() {
	LEDR.setMode(GPIOMode::OUT_PushPull);
	if (!RCC.setClock(SysclkSource::HSE)) while (true);

	LCD1602_IIC_t my_lcd(GPIOF[11], GPIOF[12], SysDelay);
	my_lcd.setMode();
	my_lcd.DrawString("Hello");
	my_lcd.DrawString("MCUDEV", 2, 1);
	while (true) {
		LEDR.Toggle();
		SysDelay(500);// ms
	}
}
