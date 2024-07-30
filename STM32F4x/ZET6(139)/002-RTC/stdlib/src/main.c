#include "main.h"


int main(void)
{
	LED_Init();
	LCD_Init();

	LCD_Clear(BLUE);
	LCD_String(10,20,"RTC test:",RED);
	if(!MyRTC_Init())LCD_String(10,50,"RTC Init OK!",RED);
	RTC_AlarmShow(10,150);
	while (1)
	{
		delay_ms(500);
		RTC_TimeShow(10,100);
		LEDTog(LED1);
	}
}


