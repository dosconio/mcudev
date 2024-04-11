
#include "MCU/ST/STM32F103VEx"
using namespace uni;

#define GPIOB_LED_R 5
#define GPIOB_LED_G 0
#define GPIOB_LED_B 1

static void light_set(unsigned led_id, bool lighton = true) {
	uni::GPIO_Pin &LED = GPIO['B'][led_id];
	LED.setMode(GPIOMode::OUT_PushPull);// Ä¬ÈÏ 2MHz
	LED = !lighton;// ¸ºÂß¼­LED
}

void LED_Light(bool red, bool green, bool blue)
{
	GPIO['B'].enClock();
	light_set(GPIOB_LED_R, red);
	light_set(GPIOB_LED_G, green);
	light_set(GPIOB_LED_B, blue);
}
