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

void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)//写入flash
{
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    

	secpos=WriteAddr/528;//页地址
	secoff=WriteAddr%528;//页内的偏移
	secremain=528-secoff;//页剩余空间大小   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于528个字节
	while(1) 
	{	
//		FlashPageRead(secpos,&AT45_buffer[0]);//读出整页内容
		FlashPageEarse(secpos);//擦除该页
		for(i=0;i<secremain;i++)	   //复制
		{
			AT45_buffer[i+secoff]=pBuffer[i];	  
		}
//		FlashPageWrite(secpos,&AT45_buffer[0]);//写入整个扇区  
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 
		   	pBuffer+=secremain;  //指针偏移  
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>528)secremain=528;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	}	 	 
}

void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)//读取flash	  
{
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    

	secpos=ReadAddr/528;//页地址
	secoff=ReadAddr%528;//页内的偏移
	secremain=528-secoff;//页剩余空间大小
	if(NumByteToRead<=secremain)secremain=NumByteToRead;
	while(1)
	{
//		FlashPageRead(secpos,&AT45_buffer[0]);
		for(i=0;i<secremain;i++)
		{
			pBuffer[i]=AT45_buffer[i+secoff];
		}
		if(NumByteToRead==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 
		   	pBuffer+=secremain;  //指针偏移  
		   	NumByteToRead-=secremain;				//字节数递减
			if(NumByteToRead>528)secremain=528;	//下一个扇区还是写不完
			else secremain=NumByteToRead;		//下一个扇区可以写完了
		}
	}
}

