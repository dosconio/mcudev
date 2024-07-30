#include "at45db.h"
#include "spi.h"
#include "led.h"
#include "lcd.h"
u8 AT45_buffer[528];

void SPI_Flash_Init(void)
{
	SPI1_Init();
	CSPin_init();
}

u16 FlashReadID(void)
{
	u16 temp;
	AT_CS_LOW();	
  	SPI1_RWByte(0x9F);
  	temp=SPI1_RWByte(Dummy);
	temp<<=8;
	temp|=SPI1_RWByte(Dummy);
  	AT_CS_HIGH();
	return temp;	
}

void FlashPageEarse(u16 page)
{	
	FlashWaitBusy();	
	AT_CS_LOW();
	SPI1_RWByte(PAGE_ERASE);
	SPI1_RWByte((u8)(page>>6));
	SPI1_RWByte((u8)(page<<2));
	SPI1_RWByte(Dummy);
	AT_CS_HIGH();	
}

void FlashPageRead1(u8* Data,u32 WriteAddr,u16 NumByteToWrite)	
{

	AT_CS_LOW();
	SPI1_RWByte(0x03);
  	SPI1_RWByte(0x00);
  	SPI1_RWByte(0x00);
  	SPI1_RWByte(0x00);
		while(NumByteToWrite--)
	{
		*Data=SPI1_RWByte(Dummy);
		Data++;
  }
	AT_CS_HIGH();	
}

void FlashPageWrite1(u8* Data,u32 WriteAddr,u16 NumByteToWrite)		
{
	u16 i=1000;

	AT_CS_LOW();
	SPI1_RWByte(0x06);
	AT_CS_HIGH();
	AT_CS_LOW();
	SPI1_RWByte(0x02);
	SPI1_RWByte(0x00);
	SPI1_RWByte(0x00);
	SPI1_RWByte(0x00);
	while(NumByteToWrite--)
	{
		SPI1_RWByte(*Data);
		Data++;
  }
	AT_CS_HIGH();		
	while(i--);
}
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
  AT_CS_LOW();
	SPI1_RWByte(0x06);
	AT_CS_HIGH();
 
  AT_CS_LOW();
  /* Send Sector Erase instruction */
  SPI1_RWByte(0xD8);
  /* Send SectorAddr high nibble address byte */
  SPI1_RWByte((SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  SPI1_RWByte((SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  SPI1_RWByte(SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  AT_CS_HIGH();

  /* Wait the end of Flash writing */
  FlashWaitBusy();
}
void FlashWaitBusy(void)
{
	u8 state_reg=0x00;
	AT_CS_LOW();	
	SPI1_RWByte(0x05);	 
	do
	{
		state_reg=SPI1_RWByte(Dummy);
//		LCD_Num(20,170,state_reg,4,GREEN);
	}while((state_reg&0x01)==SET);
	AT_CS_HIGH();
}

void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)//д��flash
{
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    

	secpos=WriteAddr/528;//ҳ��ַ
	secoff=WriteAddr%528;//ҳ�ڵ�ƫ��
	secremain=528-secoff;//ҳʣ��ռ��С   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������528���ֽ�
	while(1) 
	{	
//		FlashPageRead(secpos,&AT45_buffer[0]);//������ҳ����
		FlashPageEarse(secpos);//������ҳ
		for(i=0;i<secremain;i++)	   //����
		{
			AT45_buffer[i+secoff]=pBuffer[i];	  
		}
//		FlashPageWrite(secpos,&AT45_buffer[0]);//д����������  
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  //ָ��ƫ��  
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>528)secremain=528;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	}	 	 
}

void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)//��ȡflash	  
{
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    

	secpos=ReadAddr/528;//ҳ��ַ
	secoff=ReadAddr%528;//ҳ�ڵ�ƫ��
	secremain=528-secoff;//ҳʣ��ռ��С
	if(NumByteToRead<=secremain)secremain=NumByteToRead;
	while(1)
	{
//		FlashPageRead(secpos,&AT45_buffer[0]);
		for(i=0;i<secremain;i++)
		{
			pBuffer[i]=AT45_buffer[i+secoff];
		}
		if(NumByteToRead==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  //ָ��ƫ��  
		   	NumByteToRead-=secremain;				//�ֽ����ݼ�
			if(NumByteToRead>528)secremain=528;	//��һ����������д����
			else secremain=NumByteToRead;		//��һ����������д����
		}
	}
}

