// ARMCC-5 GB2312 @dosconio
#include "MCU/ST/STM32F103VEx"
using namespace uni;
#define GPIOB_LED_R 5
#define GPIOB_LED_G 0
#define GPIOB_LED_B 1

void light_set(unsigned led_id, bool lighton = true) {
	GPIO['B'][led_id].setMode(GPIOMode::OUT_PushPull, GPIOSpeed::Atmost_2MHz);// �������
	// GPIOSpeed �����ģʽĬ���� 2MHz���������ϵڶ�����������ʡ�ԣ�
	GPIO['B'][led_id] = !lighton;// �͵�ƽ
}

int main() {
	GPIO['B'].enClock();
	light_set(GPIOB_LED_R);
	light_set(GPIOB_LED_G, false);
	light_set(GPIOB_LED_B);
	while (true);
}
