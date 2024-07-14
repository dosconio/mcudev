// ARMCC-5 GB2312 @dosconio

#include "Device/SysTick"

#include "../cominc/lcd.h"
extern "C" {
	#include "bsp_ili9341_lcd.h"
	#include "bsp_xpt2046_lcd.h"
}
#include "main.h"
#include "./system.h"
#include "integer/ruststyle.h"
#undef GPIO_Pin
#undef GPIOA
#include "driver/Radio/TEA5767.h"
#include <stdio.h>

char buff[129];



int main() {
	Clock_Initialize();
	LCD_Initialize();
	LED_Initialize();
	KEY_Initialize();
	LCD_Clear();
	GPIO_Pin& RLED = LED_RED_REF(),
		& GLED = LED_GREEN_REF(),
		& BLED = LED_BLUE_REF();

	TEA5767_t tea(GPIOA[2], GPIOA[3], SysDelay);// SDA SCL
	tea.setFrequency(92000);
	
	u8 i = 0;
	
	if (!tea) while (true) {
		LED_RED_REF().Toggle();
		SysDelay(500);
	}
	
	
	while (true) {
		if (KEY_Scanwait(KEY_1_REF()))
			tea.SearchAuto(false);
		if (KEY_Scanwait(KEY_2_REF()))
			tea.SearchAuto(true);
		
		i++;
		SysDelay(10);
		if (i == 200)
		{
			i = 0;
			LCD_Clear();
			LCD_SetColors(Col16::ARGB(_COLOR_AliceBlue), Col16::ARGB(_COLOR_Maroon));
			LCD_SetFont(&Font16x24);
			u16 freq = tea.getFrequency();
			sprintf(buff, "Freq %dkHz", freq);
			LCD.Draw(0, 0, buff);
		}
	}
	
}

/*
case key1_val://�Զ�������һ����̨.
	if (mode == 0)//���ڽ���ģʽ��Ч
	{
		while (1)
		{
			if (freqset < 10800)freqset -= 10;   // or `+=` Ƶ������100Khz
			else freqset = 8700;				//�ص����
			RDA5820_Freq_Set(freqset);		//����Ƶ��
			delay_ms(10);					//�ȴ���Ƶ�ź��ȶ�
			if (RDA5820_RD_Reg(0X0B) & (1 << 8))//��һ����Ч��̨. 
			{
				RDA5820_Show_Msg();			//��ʾ��Ϣ
				break;
			}
			RDA5820_Show_Msg();			//��ʾ��Ϣ
			//����̨�ڼ��а�������,��������̨.
			key = Key_Scan(0);//��֧������
			if (key)break;
		}
	}
*/

