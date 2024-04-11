
#include "MCU/ST/STM32F103VEx"
using namespace uni;

#define GPIOB_LED_R 5
#define GPIOB_LED_G 0
#define GPIOB_LED_B 1

static void light_set(unsigned led_id, bool lighton = true) {
	GPIO['B'][led_id].setMode(GPIOMode::OUT_PushPull);// 推挽输出
	GPIO['B'][led_id].setSpeedM(2);// 2MHz
	GPIO['B'][led_id] = !lighton;// 低电平
}

void LED_Light(bool red, bool green, bool blue)
{
	GPIO['B'].enClock();
	light_set(GPIOB_LED_R, red);
	light_set(GPIOB_LED_G, green);
	light_set(GPIOB_LED_B, blue);
}
