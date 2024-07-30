extern "C" {
#include "main.h"
}



int main(void)
{
	LED_Init();
	LCD_Init();
	Touch_Init();
	LCD_Clear(BLUE);
	LCD_String(20,20,(u8*)"Touch screen",RED);
	LCD_String(20,80,(u8*)"X value:",GREEN);
	LCD_String(20,110,(u8*)"Y value:",GREEN);
	while(1)
	{	
		if(PEN_int==RESET)
		{
			LEDOn(LED1);
			LCD_Num(150,80,Read_XY(CMD_RDX),4,WHITE);
			LCD_Num(150,110,Read_XY(CMD_RDY),4,WHITE);
		}
		LEDOff(LED1);
	}
}


