// ARMCC-5 GBK @dosconio
#include "MCU/ST/STM32F407ZEx"
#include "driver/Video/ILI9320.h"
using namespace uni;
GPIO_Pin& LEDR = GPIOI[5];

void my_delay(stduint i) {
	for0(_i, i);
}

int main() {
	LEDR.setMode(GPIOMode::OUT_PushPull);
	if (!RCC.setClock(SysclkSource::HSE)) while (true);
	SysTick::enClock(1000);
	
	GPIO_Pin* Dats[16] = {
		&GPIOB[ 9], &GPIOH[ 7], &GPIOH[14], &GPIOH[ 6],
		&GPIOH[13], &GPIOG[ 1], &GPIOH[12], &GPIOG[ 0],
		&GPIOH[11], &GPIOF[15], &GPIOH[10], &GPIOF[14], 
		&GPIOH[ 9], &GPIOF[11], &GPIOH[ 8], &GPIOF[12] 
	};
	GPIO_Pin& CS = GPIOI[3],
		&RS = GPIOI[1],
		&WR = GPIOF[0],
		&RD = GPIOB[6],
		&RST = GPIOC[7];
	
	ILI9320_FreePins my_ili(16, Dats, WR, RD, RS, RST, CS, CS, CS);
	my_ili.func_delay_us = my_delay; // SysDelay;
	VideoControlBlock LCD(0, &my_ili);
	
	
	my_ili.setMode(16, Size2(320, 240));
	uint32 ii = 100;
	{
		my_ili.Clear(Color::AliceBlue);
		LCD.DrawLine(Point(0,0), Size2(100,27), Color::Yellow);
		LCD.Draw(Rectangle(Point(10,5), Size2(20,10), Color::MistyRose, true));// 画实心矩形
		LCD.Draw(Rectangle(Point(8,8), Size2(10,10), Color::SlateBlue, false));// 画空心矩形
		LCD.Draw(Circle(Point(50,50), 20, Color::Maroon, true));// 画实心圆
		LCD.Draw(Circle(Point(100,100), 20, Color::MistyRose, false));// 画空心圆
		// 英文字
		LCD.Draw(Point(0,112), "nihao 8x16", 0, Size2(8,16));
		LCD.Draw(Point(0,128), "nihao 12x24", 0, Size2(12,24));
		LCD.Draw(Point(0,152), "nihao 16x32", 0, Size2(16,32));
	}
	while (true) {
		// my_ili.Clear(Rangstp(ii, 100, 0xFF00));
		
		LEDR.Toggle();
		SysDelay(250*2);// ms
	}

	while (1);
}
