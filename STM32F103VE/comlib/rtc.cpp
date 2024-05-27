
#include "cinc"
#include "../cominc/rtc.h"
#include "bsp_debug_usart.h"
#include "cinc"

using namespace uni;

RTC_HandleTypeDef Rtc_Handle;

// 设置时间和日期
void RTC_CalendarConfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;

  /*##-1- Configure the Date #################################################*/
  sdatestructure.Year = YEAR;
  sdatestructure.Month = MONTH;
  sdatestructure.Date =  DATE;
  sdatestructure.WeekDay = WEEKDAY;
  
	
  if(HAL_RTC_SetDate(&Rtc_Handle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    printf("\r\n初始化日期失败\r\n");
  }

  /*##-2- Configure the Time #################################################*/
  stimestructure.Hours =HOURS;
  stimestructure.Minutes = MINUTES;
  stimestructure.Seconds = SECONDS;

  if (HAL_RTC_SetTime(&Rtc_Handle, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
  {
   printf("\r\n初始化时钟失败\r\n");;
  }

  /* 在RTC备份数据Register1中写入数据 */
  HAL_RTCEx_BKUPWrite(&Rtc_Handle,RTC_BKP_DRX,RTC_BKP_DATA);
}

// 设置时间和日期
void RTC_SetDatime(uni::datimex_t* dtx)
{
	RTC_DateTypeDef sdatestructure;
	RTC_TimeTypeDef stimestructure;
	uni::datime_t& dt = dtx->datime;

	/*##-1- Configure the Date #################################################*/
	sdatestructure.Year = dt.year % 100;// 2000~2099
	sdatestructure.Month = 1 + dt.month;
	sdatestructure.Date =  1 + dt.mday;
	sdatestructure.WeekDay = 1 + dtx->weekday;

	if(HAL_RTC_SetDate(&Rtc_Handle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
		printf("\r\n初始化日期失败\r\n");

	/*##-2- Configure the Time #################################################*/
	stimestructure.Hours = dt.hour;
	stimestructure.Minutes = dt.minute;
	stimestructure.Seconds = dt.second;

	if (HAL_RTC_SetTime(&Rtc_Handle, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
		printf("\r\n初始化时钟失败\r\n");;

	/* 在RTC备份数据Register1中写入数据 */
	HAL_RTCEx_BKUPWrite(&Rtc_Handle,RTC_BKP_DRX,RTC_BKP_DATA);
}

// 获取时间和日期 since the year just before AD1
uni::datimex_t RTC_GetDatime(void)
{
	datimex_t res;
	datime_t& rest = res.datime;
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	HAL_RTC_GetTime(&Rtc_Handle, &RTC_TimeStructure, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&Rtc_Handle, &RTC_DateStructure, RTC_FORMAT_BIN);
	
	rest.second = RTC_TimeStructure.Seconds;
	rest.minute = RTC_TimeStructure.Minutes;
	rest.hour   = RTC_TimeStructure.Hours;
	rest.mday   = RTC_DateStructure.Date - 1;
	rest.month  = RTC_DateStructure.Month - 1;
	rest.year   = RTC_DateStructure.Year;
	return res;
}

// RTC配置：选择RTC时钟源，设置RTC_CLK的分频系数
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{  
	RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*##-1- 启用PWR时钟并启用对备份域的访问###################################*/
  /* 要更改RTC功能（LSE，LSI）的源时钟，您必须：
     - 使用__HAL_RCC_PWR_CLK_ENABLE（）启用电源时钟
     - 在使用之前使用HAL_PWR_EnableBkUpAccess（）函数启用写访问 配置RTC时钟源（复位后执行一次）。
     - 使用__HAL_RCC_BACKUPRESET_FORCE（）和重置备份域
     __HAL_RCC_BACKUPRESET_RELEASE（）。
     - 配置所需的RTc时钟源*/
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();
  
  /* 为备份寄存器启用BKP CLK */
  __HAL_RCC_BKP_CLK_ENABLE();
  
  /*##-2-将ISE配置为RTC时钟源 ###################################*/
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  { 
    printf("\n\r RTC 配置失败 \r\n");;
  }
  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
   printf("\n\r RTC 时钟初始化失败 \r\n");
  }
  
  /*##-3- 启用RTC外设时钟 #######################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();

}

void RTC_Initialize() {
	Rtc_Handle.Instance = RTC; 
	Rtc_Handle.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	if (HAL_RTC_Init(&Rtc_Handle) != HAL_OK)
		while (0);
	if (false) RTC_CalendarConfig();
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)// 检查是否电源复位
		;// 发生电源复位
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
		;// 发生外部复位
	__HAL_RCC_CLEAR_RESET_FLAGS();
}


