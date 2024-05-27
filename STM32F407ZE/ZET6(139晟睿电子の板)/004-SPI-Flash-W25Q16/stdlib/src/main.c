// 20240508 û�����ɹ�

#include "main.h"
#define  FLASH_ReadAddress     0x0000000

u8 TxBuffer[3] = {0x55,0x02,0x03};
u8 RxBuffer[3] = {0};
extern u8 AT45_buffer[512];

int main(void)
{
	LED_Init();//LED��ʼ��
	LCD_Init();//LCD��ʼ��  
	SPI_Flash_Init();//SPI��ʼ��
	LCD_String(10,20,"139SR STM32F407ZET6",RED);
	LCD_String(20,50,"SPI FLASH demo",RED);
	LCD_String(20,80,"SPI FLASH Read ID:",WHITE);
	LCD_Num(20,110,FlashReadID(),6,GREEN);//��ȡID��
	LCD_String(20,140,"Write to flash...",WHITE);
	SPI_FLASH_SectorErase(FLASH_ReadAddress);//����ָ������
	FlashPageWrite1(TxBuffer,FLASH_ReadAddress,3);//д��
	LCD_String(20,170,"Read from flash:",WHITE);
	FlashPageRead1(RxBuffer,FLASH_ReadAddress,3);//����
	if(RxBuffer[1]==0x02)  //������д����Ƚϣ������ȷ������LED
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

