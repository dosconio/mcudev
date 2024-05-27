#ifndef _RTC_INC_
#define _RTC_INC_

#include "MCU/ST/STM32F103VEx"
#include "cinc"
#include "driver/RealtimeClock.h"
#include "stm32f1xx.h"
#include "cinc"

//�Ƿ�ʹ��LCD��ʾ
#define USE_LCD_DISPLAY

extern RTC_HandleTypeDef Rtc_Handle;
// ʱ��Դ�궨��
#define RTC_CLOCK_SOURCE_LSE      
//#define RCC_OSCILLATORTYPE_LSI  
 
#define HOURS                     0x12          // 0~23
#define MINUTES                   0x12          // 0~59
#define SECONDS                   0x12          // 0~59


// ���ں궨��  ����WEEKDAYΪ��Ч�궨�壬ֻҪ���������գ�HAL����Զ������ WEEKDAY
#define WEEKDAY                   6           // 1~7
#define DATE                      0x24        // 1~31
#define MONTH                     0x12        // 1~12
#define YEAR                      0x18        // 0~99

// ������Ĵ����궨��
#define RTC_BKP_DRX          RTC_BKP_DR1
// д�뵽���ݼĴ��������ݺ궨��
#define RTC_BKP_DATA         0X32F2 

//{TEMP}
extern "C" {

void RTC_CalendarConfig(void);
void RTC_SetDatime(uni::datimex_t* dtx);
uni::datimex_t RTC_GetDatime(void);
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc);

void RTC_Initialize();
}
	
#endif
