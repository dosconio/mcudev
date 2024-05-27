// ARMCC-5 GB2312 @dosconio
#include "MCU/ST/STM32F407ZEx"
extern "C" {
#include <stm32f4xx_it.h>
#include "delay.h"
}
using namespace uni;
#define GPIOB_LED 0

void light_set(unsigned led_id, bool lighton = true) {
	GPIO['B'][led_id].setMode(GPIOMode::OUT_PushPull);// 推挽输出
	GPIO['B'][led_id] = !lighton;// 低电平
}

void light_tog(unsigned led_id) {
	GPIO['B'][led_id].Toggle();
}

//extern "C" void SysTick_Handler(void);

int main() {

	GPIO['A'].enClock();
	GPIO['A'][0].setMode(GPIOMode::IN_Floating);
	GPIO['B'].enClock();
	light_set(GPIOB_LED, false);
	while (true) {
		delay_ms(500);
		if (!GPIO['A'][0]);
		else {
			light_tog(GPIOB_LED);
		}
	}
}

extern "C" u16 ntime;
void SysTick_Handler(void)
{
	ntime--;
}
