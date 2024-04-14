

#include "../cominc/led.h"

using namespace uni;

static void light_set(unsigned led_id, bool lighton = true) {
	uni::GPIO_Pin &LED = GPIO['B'][led_id];
	LED.setMode(GPIOMode::OUT_PushPull);// Ä¬ÈÏ 2MHz
	LED = !lighton;// ¸ºÂß¼­LED
}

//

void LED_Initialize() {
	light_set(GPIOB_LED_R, false);
	light_set(GPIOB_LED_G, false);
	light_set(GPIOB_LED_B, false);
}

uni::GPIO_Pin& LED_RED_REF() {
	return GPIO['B'][GPIOB_LED_R];
}

uni::GPIO_Pin& LED_GREEN_REF() {
	return GPIO['B'][GPIOB_LED_G];
}

uni::GPIO_Pin& LED_BLUE_REF() {
	return GPIO['B'][GPIOB_LED_B];
}

void LED_Light(bool red, bool green, bool blue)
{
	GPIO['B'].enClock();
	light_set(GPIOB_LED_R, red);
	light_set(GPIOB_LED_G, green);
	light_set(GPIOB_LED_B, blue);
}
