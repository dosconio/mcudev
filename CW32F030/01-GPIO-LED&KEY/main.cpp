#include <MCU/XY/CW32F030>
// @dosconio 20240716
// CW32F030C8 小蓝板
extern "C" {
	// if mixed using with Official Library
}
int main() {
	using namespace uni;
	// RCC_HSI_Enable(RCC_HSIOSC_DIV6);
	// InitTick(8000000);
	GPIO_Pin& LED = GPIO['C'][13];
	GPIO_Pin& KEY = GPIOB[2];
	LED.setMode(GPIOMode::OUT_PushPull);
	KEY.setMode(GPIOMode::IN);
	while (true) {
		LED = KEY.getInn();// 不使用 `LED=KEY` 防止被优化，等价于 `bool b=KEY;LED=KEY;`
	}
}
