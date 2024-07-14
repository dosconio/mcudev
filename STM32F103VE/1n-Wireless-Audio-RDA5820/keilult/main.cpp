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
#include "driver/Radio/RDA5820.h"
#include <stdio.h>

char buff[129];
RDA5820_t* glb_rda;

void RDA5820_Show_Msg(void)
{
	LCD_Clear();
	LCD_SetColors(Col16::ARGB(_COLOR_AliceBlue), Col16::ARGB(_COLOR_Maroon));
	LCD_SetFont(&Font16x24);
	//
	u16 freq = glb_rda->getFrequency();
	sprintf(buff, "Freq %.2fkHz", (float)freq*10);
	LCD.Draw(0, 0, buff);
	//
	u8 rssi = glb_rda->getRSSI();
	sprintf(buff, "RSSI %f", (float)rssi);
	LCD.Draw(0, 24*1, buff);
}

void UpdateFreq(bool dir) {
	u16 freq = glb_rda->getFrequency();
	if (dir) {
		if(8700 <= freq && freq < 10800) freq += 10;// 100Khz
		else freq = 8700;
	}
	else {
		if(freq > 8700) freq -= 10;// 100Khz
		else freq = 10800;
	}
	glb_rda->setFreqency(freq);
	RDA5820_Show_Msg();
}

int main() {
	Clock_Initialize();
	LCD_Initialize();
	LED_Initialize();
	KEY_Initialize();
	LCD_Clear();
	GPIO_Pin& RLED = LED_RED_REF(),
		& GLED = LED_GREEN_REF(),
		& BLED = LED_BLUE_REF();

	u8 i = 0;
	
	RDA5820_t rda(GPIOA[2], GPIOA[3], SysDelay);// SDA SCL
	glb_rda = &rda;
	if (!rda) while (true) {
		LED_RED_REF().Toggle();
		SysDelay(500);
	}
	
	rda.setBand(0);//����Ƶ��Ϊ87~108Mhz
	rda.setSpace(0);//���ò���Ϊ100Khz
	rda.setMode(false, 3, 63);// receive gain-3 max-power	

	u16 freqset = 10000;// Ĭ��Ϊ93.6Mhz
	rda.setFreqency(freqset);
	RDA5820_Show_Msg();
	
	while (true) {
		if (KEY_Scanwait(KEY_1_REF()))
			UpdateFreq(false);
		if (KEY_Scanwait(KEY_2_REF()))
			UpdateFreq(true);
		
		i++;
		SysDelay(10);
		if (i == 200) // ÿ�������Ҽ��һ���ź�ǿ�ȵ���Ϣ.
		{
			i = 0;
			RDA5820_Show_Msg();
			LED_GREEN_REF().Toggle();
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

