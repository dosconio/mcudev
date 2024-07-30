#include "rtc.h"
#include "lcd.h"
#include "key.h"

static RTC_TimeTypeDef RTC_TimeStructure;
static RTC_InitTypeDef RTC_InitStructure;
static RTC_AlarmTypeDef  RTC_AlarmStructure;
static __IO uint32_t AsynchPrediv=0,SynchPrediv=0;

u8 MyRTC_Init(void)
{
  	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)//δ����ʱ��
  	{  
    	RTC_Config();
    	RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;//RTC �첽���� ��<0X7F��
    	RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;//RTC ͬ������ ��<0X7FFF��
    	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;//24Сʱ��
    	if(RTC_Init(&RTC_InitStructure) == ERROR)return 1;
    	RTC_TimeRegulate();//����ʱ�� 
  	}
  	else//������ʱ��
  	{   
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	    PWR_BackupAccessCmd(ENABLE);//ʹ��RTC����
	    RTC_WaitForSynchro();//�ȴ�RTC APB�Ĵ���ͬ��
	    RTC_ClearFlag(RTC_FLAG_ALRAF);//���RTC���ӱ�־
	    EXTI_ClearITPendingBit(EXTI_Line17);//����ж���17��־���ڲ�������RTC���ӣ�
  	}
	return 0;
}

void RTC_Config(void)
{
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  	PWR_BackupAccessCmd(ENABLE);//ʹ��RTC����
  	RCC_LSEConfig(RCC_LSE_ON);//ʹ���ⲿ����
  	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);//�ȴ��ⲿ����׼����
  	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//ѡ��RTCʱ��Դ  
  	SynchPrediv=0xFF;
  	AsynchPrediv=0x7F; 
  	RCC_RTCCLKCmd(ENABLE);//ʹ��RTCʱ��
  	RTC_WaitForSynchro();//�ȴ�RTC APB�Ĵ���ͬ��
}

void RTC_TimeShow(u16 x,u16 y)
{
  	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	LCD_String(x,y,"Time:   :  :",RED);
	LCD_Num(x+95,y,RTC_TimeStructure.RTC_Hours,2,RED);
	LCD_Num(x+145,y,RTC_TimeStructure.RTC_Minutes,2,RED);
	LCD_Num(x+195,y,RTC_TimeStructure.RTC_Seconds,2,RED);
}

void RTC_TimeRegulate(void)
{
  	u32 tmp_hh = 0x01, tmp_mm = 0x01, tmp_ss = 0x01;
	RTC_TimeStructure.RTC_H12= RTC_H12_AM;		

   	RTC_TimeStructure.RTC_Hours = tmp_hh;
    RTC_TimeStructure.RTC_Minutes = tmp_mm;
    RTC_TimeStructure.RTC_Seconds = tmp_ss;	
  	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure)!=ERROR)
  	RTC_WriteBackupRegister(RTC_BKP_DR0,0x32F2);
	LCD_Clear(BLUE);
  	tmp_hh = 0x01;
  	tmp_mm = 0x01;
  	tmp_ss = 0x01;
  	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
  	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
  	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 2;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = tmp_mm;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = tmp_ss;
  	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
  	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
  	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);//����RTC���ӼĴ���
  	RTC_ITConfig(RTC_IT_ALRA, ENABLE);//ʹ������A���ж�
  	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);//ʹ������A
	LCD_Clear(BLUE);
}

void RTC_AlarmShow(u16 x,u16 y)
{
  	RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
	LCD_String(x,y,"Alarm:  :  :",RED);
	LCD_Num(x+95,y,RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours,2,RED);
	LCD_Num(x+145,y,RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes,2,RED);
	LCD_Num(x+195,y,RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds,2,RED);
}
