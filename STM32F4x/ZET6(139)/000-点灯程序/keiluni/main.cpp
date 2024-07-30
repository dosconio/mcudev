// ARMCC-5 GB2312 @dosconio
#include "MCU/ST/STM32F407ZEx"
using namespace uni;
#define GPIOB_LED 0

void light_set(unsigned led_id, bool lighton = true) {
	GPIO['B'][led_id].setMode(GPIOMode::OUT_PushPull);// 推挽输出
	GPIO['B'][led_id] = !lighton;// 低电平
}

int main() {
	GPIO['B'].enClock();
	light_set(GPIOB_LED);
	while (true);
}
