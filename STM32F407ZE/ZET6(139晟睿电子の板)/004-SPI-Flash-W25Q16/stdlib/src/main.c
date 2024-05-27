// 20240508 没有做成功

#include "main.h"
#define  FLASH_ReadAddress     0x0000000

u8 TxBuffer[3] = {0x55,0x02,0x03};
u8 RxBuffer[3] = {0};
extern u8 AT45_buffer[512];

int main(void)
{
	LED_Init();//LED初始化
	LCD_Init();//LCD初始化  
	SPI_Flash_Init();//SPI初始化
	LCD_String(10,20,"139SR STM32F407ZET6",RED);
	LCD_String(20,50,"SPI FLASH demo",RED);
	LCD_String(20,80,"SPI FLASH Read ID:",WHITE);
	LCD_Num(20,110,FlashReadID(),6,GREEN);//读取ID号
	LCD_String(20,140,"Write to flash...",WHITE);
	SPI_FLASH_SectorErase(FLASH_ReadAddress);//擦除指定扇区
	FlashPageWrite1(TxBuffer,FLASH_ReadAddress,3);//写入
	LCD_String(20,170,"Read from flash:",WHITE);
	FlashPageRead1(RxBuffer,FLASH_ReadAddress,3);//读出
	if(RxBuffer[1]==0x02)  //读出与写入相比较，如果正确，点亮LED
	{
		LCD_String(20,200,"read success",RED);
		LEDOff(LED2);
	}
	else LCD_String(20,200,"read  failure",RED);
	while(1)
	{
	//	LEDOff(LED2);
	//	delay_ms(500);
	//	LEDTog(LED2);
	}
}

