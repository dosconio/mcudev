#ifndef _AT45DB_H_
#define _AT45DB_H_  

#include "stm32f4xx_conf.h"

#define FLASH_CHREAD 			0x0B
#define FLASH_CLREAD 			0x03
#define FLASH_PREAD				0xD2
#define FLASH_BUFWRITE1 		0x84
#define FLASH_IDREAD 			0x9F
#define FLASH_STATUS 			0xD7
#define PAGE_ERASE 				0x81
#define PAGE_READ 				0xD2
#define BUFFER_2_WRITE 			0x87	// д��ڶ�������
#define B2MM_PAGEERASE 			0x86	// ���ڶ�������������д�����洢��������ģʽ��
#define Dummy 					0xA5

#define AT_CS_LOW()     	GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define AT_CS_HIGH()   		GPIO_SetBits(GPIOA, GPIO_Pin_4)

void SPI_Flash_Init(void);	         	//SPI��ʼ��
void FlashPageEarse(u16 page);			//����ָ����ҳ,ҳ��Χ0-4095
//void FlashPageRead(u16 page,u8 *Data);	//��ȡ��ҳ��ҳ��Χ0-4095
//void FlashPageWrite(u16 page,u8 *Data);	//дһ��ҳ��ҳ��Χ0-4095
void FlashPageWrite1(u8* Data,u32 WriteAddr,u16 NumByteToWrite);
void FlashPageRead1(u8* Data,u32 WriteAddr,u16 NumByteToWrite);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void FlashWaitBusy(void);			    
u16  FlashReadID(void);					//��ȡflashID�����ֽ�
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);	//д��flash
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);	//��ȡflash

#endif


