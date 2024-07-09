// ARMCC-5 GB2312 @dosconio
#include "MCU/ST/STM32F407ZEx"
extern "C" {
	#include "delay.h"
	//#include "stm32f4xx_it.h"
	//#include "stm32f4xx_conf.h"
}
using namespace uni;
#define GPIOB_LED 0

typedef GeneralPurposeInputOutputPin sbit;
#if 0
#undef GPIOA
#undef GPIOB
#undef GPIOC
#undef GPIOD
#undef SDIO
#undef RESET
#endif

// AD9952
/*


D15 D14
D13 D12
D11 D10
D09 D08
B15 B14
B13 B12
右下角
*/

unsigned char ControlWord[] =
{
	0x00,//0A   第0组：功能控制字
	0x10,//0B
	0x00,//0C
	0x00,//0D

	0xA7,//1A   第1组：模拟部分控制字  采用20M有源输入，20倍频，VCO范围设置为250M到400MHz，锁相环泵电流为150UA
	0x00,//1B
	0x00,//1C

	0x00,//2A   第2组：ASF
	0x00,//2B

	0x00,//3A   第3组：ARR

	0x0A,//4A 第4组：频率控制字
	0xD7,//4B
	0xA3,//4C
	0x00,//4D

	0x00,//5A	第5  相位控制字                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             组：相位偏移字
	0x00,//5B
};

typedef byte uchar;
sbit &PWRDWNCTL = GPIOB[15];
sbit &RST = GPIOD[9];
sbit &IOSYNC  = GPIOD[11];
sbit &SDO = GPIOD[13];
sbit &CS = GPIOD[15];
sbit &SCLK = GPIOB[12];
sbit &SDIO = GPIOB[14];
sbit &OSK = GPIOD[8];
sbit &UPDATA = GPIOD[10];
sbit* sbits[] = {
	&PWRDWNCTL,&RST,&IOSYNC,&SDO,&CS,&SCLK,&SDIO,&OSK,&UPDATA
};
void AD9952WriteByte(byte commond, byte* databyte, byte lengh)
{
	uchar SPDR;

	for0 (i,8)
	{
		SCLK = 0;
		SDIO = 0 != (commond & 0x80);
		SCLK = 1;
		commond <<= 1;
	}

	for0r(j, lengh)//写lengh位的data
	{
		SPDR = databyte[j];	    //取出高 byte
		for0 (i,8)
		{
			SCLK = 0;
			SDIO = SPDR & 0x80;
			SCLK = 1;
			SPDR <<= 1;
		}
	}
}

/*
void Write_Amplitude(unsigned  int  Ampli)
{
	unsigned char buf[] = {0x40,0x20,0x00,0x02};
	// unsigned char buf[] = {0x02,0x00,0x20,0x40};
	uint A_temp = 0x3fff;
    A_temp=Ampli;
//  ControlWord[7]=(uchar)A_temp;
//  ControlWord[8]=(uchar)(A_temp>>8);
//  AD9952WriteByte(0X02,ControlWord+7,2);
//	

	CS_LOW();
	AD9952WriteByte(0x00, buf, 4);
	ControlWord[7] = (uchar)A_temp;
   ControlWord[8]=(uchar)(A_temp>>8) & 0x3f;
   AD9952WriteByte(0x02,ControlWord+7,2);
   //AD9952WriteByte(0x03,ControlWord+7,1);
//    CS_HIGH();
	CS_HIGH();

  //UPDATA = 1;
  //UPDATA = 0;
	UPDATA_HIGH();
	UPDATA_LOW();
}
*/

void Init_AD9952(void) //初始化AD9952使输出为1MHZ
{
	PWRDWNCTL = 0;
	CS = 0;
	UPDATA = 0;
	IOSYNC = 0;
	RST = 1;
	delay_ms(5);
	RST = 0;

	//复位
	AD9952WriteByte(0X00, ControlWord, 4);
	AD9952WriteByte(0X01, ControlWord + 4, 3);
	AD9952WriteByte(0X02, ControlWord + 7, 2);
	AD9952WriteByte(0X03, ControlWord + 9, 1);
	AD9952WriteByte(0X04, ControlWord + 10, 4);
	AD9952WriteByte(0X05, ControlWord + 14, 2);

	UPDATA = 1;
	UPDATA = 0;
}

//函数名称：void Freq_convert(ulong Freq)
//函数功能：正弦信号频率数据转换
//入口参数：Freq  需要转换的频率，取值从0—SYSCLK/2
//出口参数：无   但是影响全局变量ControlWord[10-13]的值
//==============================================================================================
void Freq_convert(uint32 Freq)
{
	uint32 Temp;
	Temp = (uint32)Freq * 10.73741824;	   //将输入频率因子分为四个字节  10.73741824=(2^32)/400000000
	ControlWord[10] = (uchar)Temp;
	ControlWord[11] = (uchar)(Temp >> 8);
	ControlWord[12] = (uchar)(Temp >> 16);
	ControlWord[13] = (uchar)(Temp >> 24);
	AD9952WriteByte(0X04, ControlWord + 10, 4);
	AD9952WriteByte(0X05, ControlWord + 14, 2);
	UPDATA = 1;
	UPDATA = 0;
}

void light_set(unsigned led_id, bool lighton = true) {
	GPIO['B'][led_id].setMode(GPIOMode::OUT_PushPull);// 推挽输出
	GPIO['B'][led_id] = !lighton;// 低电平
}

int main() {
	for0 (i, numsof(sbits)) 
		sbits[i]->setMode(GPIOMode::OUT_PushPull);
	Init_AD9952();
	Freq_convert(200000);
	
	while (true);
}
/*
extern "C" u16 ntime;
void SysTick_Handler(void)
{
	ntime--;
}
*/

