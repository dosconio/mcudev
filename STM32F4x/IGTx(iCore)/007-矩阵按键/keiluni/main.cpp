// ARMCC-5 GBK @dosconio 20240726
#include "MCU/ST/STM32F4"
#include "driver/LCD1602.h"
#include "driver/Mtrkbd.h"
#define g(x) gpo(x)

using namespace uni;
GPIO_Pin& LEDR = GPIOI[5];
GPIO_Pin& kr1=g(C)[6], &kr2=g(A)[10], &kr3=g(A)[12], &kr4=g(I)[2];
GPIO_Pin& kc1=g(G)[3], &kc2=g(A)[8], &kc3=g(B)[15], &kc4=g(I)[0];

void delay_010ms(void) { SysDelay(10); }

int main() {
	LEDR.setMode(GPIOMode::OUT_PushPull);
	if (!RCC.setClock(SysclkSource::HSE)) while (true);

	LCD1602_IIC_t my_lcd(GPIOF[11], GPIOF[12], SysDelay);
	Mtrkbd_4x4 kbd(kr1,kr2,kr3,kr4,kc1,kc2,kc3,kc4);	 kbd.setMode(); kbd.func_delay = delay_010ms;
	my_lcd.setMode();
	kbd.setMode();
	my_lcd.DrawString("UNI::MTRKBD4x4", 0, 0);
	
	while (true) {
		word keymap = kbd.Scan();
		char keybuf[17] {0};
		for0(i, 16) {
			keybuf[i] = '0' + !!(keymap & (1 << i)) ;
		}
		my_lcd.DrawString(keybuf, 0, 1);
	}
}
