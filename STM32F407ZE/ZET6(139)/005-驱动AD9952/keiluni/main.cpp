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
���½�
*/

unsigned char ControlWord[] =
{
	0x00,//0A   ��0�飺���ܿ�����
	0x10,//0B
	0x00,//0C
	0x00,//0D

	0xA7,//1A   ��1�飺ģ�ⲿ�ֿ�����  ����20M��Դ���룬20��Ƶ��VCO��Χ����Ϊ250M��400MHz�����໷�õ���Ϊ150UA
	0x00,//1B
	0x00,//1C

	0x00,//2A   ��2�飺ASF
	0x00,//2B

	0x00,//3A   ��3�飺ARR

	0x0A,//4A ��4�飺Ƶ�ʿ�����
	0xD7,//4B
	0xA3,//4C
	0x00,//4D

	0x00,//5A	��5  ��λ������                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             �飺��λƫ����
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

	for0r(j, lengh)//дlenghλ��data
	{
		SPDR = databyte[j];	    //ȡ���� byte
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

void Init_AD9952(void) //��ʼ��AD9952ʹ���Ϊ1MHZ
{
	PWRDWNCTL = 0;
	CS = 0;
	UPDATA = 0;
	IOSYNC = 0;
	RST = 1;
	delay_ms(5);
	RST = 0;

	//��λ
	AD9952WriteByte(0X00, ControlWord, 4);
	AD9952WriteByte(0X01, ControlWord + 4, 3);
	AD9952WriteByte(0X02, ControlWord + 7, 2);
	AD9952WriteByte(0X03, ControlWord + 9, 1);
	AD9952WriteByte(0X04, ControlWord + 10, 4);
	AD9952WriteByte(0X05, ControlWord + 14, 2);

	UPDATA = 1;
	UPDATA = 0;
}

//�������ƣ�void Freq_convert(ulong Freq)
//�������ܣ������ź�Ƶ������ת��
//��ڲ�����Freq  ��Ҫת����Ƶ�ʣ�ȡֵ��0��SYSCLK/2
//���ڲ�������   ����Ӱ��ȫ�ֱ���ControlWord[10-13]��ֵ
//==============================================================================================
void Freq_convert(uint32 Freq)
{
	uint32 Temp;
	Temp = (uint32)Freq * 10.73741824;	   //������Ƶ�����ӷ�Ϊ�ĸ��ֽ�  10.73741824=(2^32)/400000000
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
	GPIO['B'][led_id].setMode(GPIOMode::OUT_PushPull);// �������
	GPIO['B'][led_id] = !lighton;// �͵�ƽ
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

