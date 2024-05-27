
#include "dev.h"
extern "C" {
	#include <stm32f1xx_hal.h>
	//#include "stm32f1xx_hal_rcc.h"
	#include "stm32f1xx_hal_adc.h"
	
	DMA_HandleTypeDef hdma_adcx, DMA_Init_Handle;
	ADC_HandleTypeDef hadc,        hadc1, ADC_Handle; // ADC_Handle
	uint16_t ADC_ConvertedValue[8];
}

#include "stm32f1xx_hal.h"

static void MX_ADC1_Init(void)
{
  

  hadc.Instance = ADC1;
  hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
	while(1);

}


#undef GPIOA
using namespace uni;

//ADCÓ²¼þ³õÊ¼»¯ HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
void MQ135_Initialize()
{
    __HAL_RCC_ADC1_CLK_ENABLE();	
	
	// Rheostat_ADC_GPIO_Config
	GPIOA.enClock();
	GPIOA[2].setMode(GPIOMode::IN_Analog);
	GPIOA[3].setMode(GPIOMode::IN_Analog);

	MX_ADC1_Init();
}

uint16 MQ135_GetAnalog() {
	ADC_ChannelConfTypeDef sConfig = {0};
	
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	while(1);
  
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
   ADC_ConvertedValue[0] = HAL_ADC_GetValue(&hadc);
    HAL_ADC_Stop(&hadc);
  
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	while(1);
  
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
   ADC_ConvertedValue[1] = HAL_ADC_GetValue(&hadc);
    HAL_ADC_Stop(&hadc);
  
	
	return ADC_ConvertedValue[0];
}

