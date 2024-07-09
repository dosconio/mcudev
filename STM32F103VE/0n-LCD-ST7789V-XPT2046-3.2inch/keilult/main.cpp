// ARMCC-5 GB2312 @dosconio
#include "main.h"
#include "Device/SysTick"
#include "dev.h"
extern "C" {
	#include "bsp_ili9341_lcd.h"// 240x320 : ILI9341 not ST7789V LCD driver and XPT2046 Touch Pad driver
	#include "bsp_xpt2046_lcd.h"
	#include <stdio.h>
	void DEBUG_USART_Config(); 
	void Usart_SendString(uint8_t *str);
}

using namespace uni;

datime_t dt_alarm = {12,21,22};

char buff[129];// used by `sprintf()`

DisplayColor Col_Highlight = GrafColor32(_COLOR_Red);
uni::GeneralPurposeInputOutputPin& BEEP = GPIO['A'][8];

void onSetting() {
	LCD_Clear();
	LCD_SetColors(Col16::ARGB(_COLOR_AliceBlue), Col16::ARGB(_COLOR_Silver));	
	LCD_SetTextColor(Col16::ARGB(_COLOR_Maroon));
	LCD_SetFont(&Font16x24);
	LCD.Draw(0, 0, "Alarm");
	LCD_SetFont(&Font24x32);
	LCD.Draw(48, 80-32, " +  +  + ");
	LCD.Draw(48, 80+32, " -  -  - ");
	
	LCD_SetColors(Col16::ARGB(_COLOR_Red), Col16::ARGB(_COLOR_AliceBlue));
	LCD_SetFont(&Font16x24);
	LCD.Draw(0, -24, "Hompage");
	
	stduint i = 1000;
	while (true) {
		XPT2046_TouchEvenHandler();
		if (pressed) {
			//[DBG] printf("%d, %d\n", (int)CrtPoint.x, (int)CrtPoint.y);
			//{TODO} 坐标有点与坐标轴不一致
			if (CrtPoint.x >= 205 && CrtPoint.y <= 35) {
				printf("按下了返回按钮");
				return;
			}
			else if (CrtPoint.x >= 85 && CrtPoint.x <= 110 &&
				CrtPoint.y >= 165 && CrtPoint.y <= 195) {
				SysDelay(100);
				dt_alarm.second++;
				if (dt_alarm.second >= 60) dt_alarm.second = 0;
				i = 1000;
			}
			else if (CrtPoint.x >= 155 && CrtPoint.x <= 175 &&
				CrtPoint.y >= 165 && CrtPoint.y <= 195) {
				SysDelay(100);
				dt_alarm.minute++; 
				if (dt_alarm.minute >= 60) dt_alarm.minute = 0;
				i = 1000;
			}
			else if (CrtPoint.x >= 215 && CrtPoint.x <= 245 &&
				CrtPoint.y >= 165 && CrtPoint.y <= 195) {
				SysDelay(100);
				dt_alarm.hour++;
				if (dt_alarm.hour >= 24) dt_alarm.hour = 0;
				i = 1000;
			}
			else if (CrtPoint.x >= 85 && CrtPoint.x <= 110 &&
				CrtPoint.y >= 100 && CrtPoint.y <= 130) {
				SysDelay(100);
				if (dt_alarm.second == 0) dt_alarm.second = 60;
				dt_alarm.second--;
				i = 1000;
			}
			else if (CrtPoint.x >= 155 && CrtPoint.x <= 175 &&
				CrtPoint.y >= 100 && CrtPoint.y <= 130) {
				SysDelay(100);
				if (dt_alarm.minute == 0) dt_alarm.minute = 60;
				dt_alarm.minute--;
				i = 1000;
			}
			else if (CrtPoint.x >= 215 && CrtPoint.x <= 245 &&
				CrtPoint.y >= 100 && CrtPoint.y <= 130) {
				SysDelay(100);
				if (dt_alarm.hour == 0) dt_alarm.hour = 24;
				dt_alarm.hour--;
				i = 1000;
			}
		}
		if (i++ >= 20 && !(i = 0)) {
			LCD_SetColors(Col16::ARGB(_COLOR_AliceBlue), Col16::ARGB(_COLOR_Silver));	
			LCD_SetTextColor(Col16::ARGB(_COLOR_Maroon));
			LCD_SetFont(&Font24x32);
			sprintf(buff, "%02u:%02u:%02u", dt_alarm.hour, dt_alarm.minute, dt_alarm.second);
			LCD.Draw(48, 80, buff);
		}
		SysDelay(50);// 没必要太高
	}
}

void onMain() {
	if (false) {
		LCD_SetFont(&Font8x16);
		LCD_SetColors(Col16::ARGB(_COLOR_AliceBlue), Col16::ARGB(_COLOR_Silver));
		LCD.Draw(0, 0, "@dosconio");
	}
	LCD_SetColors(Col16::ARGB(_COLOR_Red), Col16::ARGB(_COLOR_AliceBlue));
	LCD_SetFont(&Font16x24);
	LCD.Draw(0, -24, "Setting");
	LCD.Draw(-16*7, -24, "Utility");
}

void onDraw() {
	MQ135_GetAnalog();
	datime_t now = RTC_GetDatime().datime;
	now.year=2024-1900, now.month=5-1, now.mday=14-1;
	uint16 MQ135_Dat = ADC_ConvertedValue[0];//MQ135_GetAnalog();
	
	static const char* Weekdays[] = {
		"SUN", "MON", "TUE", "MAR", "THU", "FRI", "SAT"
	};
	LCD_SetColors(Col16::ARGB(_COLOR_AliceBlue), Col16::ARGB(_COLOR_Silver));	
	LCD_SetTextColor(Col16::ARGB(MQ135_Dat*100/4096 > 65 ? 
		_COLOR_Red : _COLOR_Maroon));
	// MQ-135
	LCD_SetFont(&Font16x24);
	//sprintf(buff, "AIR %02hu%% ? ADC %02hu%%", MQ135_Dat*100/4096, ADC_ConvertedValue[1]*100/4096);
	sprintf(buff, "AIR %02hu%% 32`C", MQ135_Dat*100/4096);
	LCD.Draw(0, 0, buff);
	LCD.Draw(0, 24, "Sunny 24`C");
	sprintf(buff, (ADC_ConvertedValue[1]*100/4096 > 94) ? "Heart 0 BPM" :
		"Heart %u BPM", 6400/(ADC_ConvertedValue[1]*100/4096));
	LCD.Draw(0, 24*2, buff);
	LCD_SetFont(&Font8x16);
	LCD_SetColors(Col16::ARGB(_COLOR_AliceBlue), Col16::ARGB(_COLOR_Maroon));
	LCD.Draw(GrafRect(0, 80+64, LCD.getLimitX(), 16, DisplayColorFrom32(_COLOR_Maroon)));
	LCD.Draw(8*4, 80+64, "! 05/14 20:13 - Homework Deadline");

	LCD_SetColors(Col16::ARGB(_COLOR_AliceBlue), Col16::ARGB(_COLOR_Silver));		
	
	LCD_SetFont(&Font24x32);
	// Show date and time
	sprintf(buff, "%02hhu:%02hhu:%02hhu", now.hour, now.minute, now.second);
	LCD.Draw(48, 80, buff);
	LCD_SetFont(&Font16x24);
	sprintf(buff, "%04hd/%02hhu/%02hhu %s", now.year+1900, now.month+1, now.mday+1, 
		Weekdays[weekday(now.year+1900, now.month+1, now.mday+1)]);
	LCD.Draw(48, 80+32, buff);
	
	if (now.hour == dt_alarm.hour && 
		now.minute == dt_alarm.minute && 
		now.second == dt_alarm.second) {
		GPIO['A'][8] = true;
		SysDelay(250);
		GPIO['A'][8] = false;
	}
}

int main() {
	SystemClock_Config();
	LED_Initialize();
	KEY_Initialize();
	LCD_Initialize();
	RTC_Initialize();
	MQ135_Initialize();
	BEEP.setMode(GPIOMode::OUT_PushPull); BEEP = false;
	DEBUG_USART_Config(); Usart_SendString((uint8_t *)"多功能桌上电子时钟 @dosconio\n");
	
	LCD_Clear();
	onMain();
	
	if (false) {
		// Draw Line
		LCD.DrawLine(DisplayPoint(16,16), DisplaySize(1,16));
		LCD.DrawLine(DisplayPoint(16,16), DisplaySize(16,1));
		LCD.DrawLine(DisplayPoint(16,16), DisplaySize(16,8), &Col_Highlight);
		LCD.DrawLine(DisplayPoint(16,16), DisplaySize(8,16), &Col_Highlight);
		// Draw Hollow Rectangle
		LCD.Draw(GrafRectHollow(32, 16, 16, 16, GrafColor32(_COLOR_MistyRose)));
	}
	stduint i = 0;
	onDraw();
	while (true) {
		XPT2046_TouchEvenHandler();
		if (pressed) {
			//[DBG] printf("%d, %d\n", (int)CrtPoint.x, (int)CrtPoint.y);
			//{TODO} 坐标有点与坐标轴不一致
			if (CrtPoint.x >= 205 && CrtPoint.y <= 35) {
				printf("按下了设置按钮");
				onSetting();
				LCD_Clear();
				while (pressed) {
					XPT2046_TouchEvenHandler();
					SysDelay(100);
				}
				printf("放下了设置按钮");
				onMain();
				i = 1000; continue;
			}
			else if (CrtPoint.x <= 120 && CrtPoint.y <= 35) {
				printf("按下了工具按钮");
				while (pressed) {
					XPT2046_TouchEvenHandler();
					SysDelay(100);
					
				}
				printf("放下了设置按钮");
				onMain();
				i = 1000; continue;
			}
		}
		else if (i++ >= 20 && !(i = 0)) onDraw();
		SysDelay(50);// 没必要太高
	}
}

void SystemClock_Config(void)
{
	if (!(RCC.HSE.setMode() && RCC.PLL.setMode()))// PLL<<HSE*9/1
		while (true);
	if (RCC.setFlash() && RCC.AHB.setMode() && RCC.Sysclock.setMode() &&
		RCC.setFlash(true) && RCC.APB1.setMode(1) && RCC.APB2.setMode(0)   );
	else while (true);
	// Update the SystemCoreClock global variable
	SystemCoreClock = HAL_RCC_GetSysClockFreq() >> //{TODO}
		AHBPrescTable[(RCC.Cfgreg & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos];
	SysTick::enClock(SystemCoreClock, 1000);// T = 1ms //{TODO} Auto get `SystemCoreClock`
}

#include <cinc>

void USART1_IRQHandler(void)
{
	UART_HandleTypeDef UartHandle;
	uint8_t ch=0; 
	if(__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_RXNE) != RESET)
	{		
		ch=( uint16_t)READ_REG(UartHandle.Instance->DR);
		WRITE_REG(UartHandle.Instance->DR,ch + 1); 
	}
}

void ADC1_IRQHandler(void)
{
	extern ADC_HandleTypeDef hadc1;
	HAL_ADC_IRQHandler(&hadc1);
}

#include <cinc>
