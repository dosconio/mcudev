// ARMCC-5 GB2312 @dosconio
// AD9951 and 9952 and 9954 (SPI)
#include "MCU/ST/STM32F4"

using namespace uni;

typedef GeneralPurposeInputOutputPin sbit;

class AD9954 {
protected:
	void Send(byte cmd, byte* databyte, byte lengh)
	{
		byte SPDR;
		for0(i, _BYTE_BITS_) {
			SCLK = 0;
			SDIO = (cmd & 0x80);
			SCLK.Toggle();
			cmd <<= 1;
		}
		for0r(j, lengh) {
			SPDR = databyte[j];	    //ȡ���� byte
			for0(i, 8)
			{
				SCLK = 0;
				SDIO = SPDR & 0x80;
				SCLK.Toggle();
				SPDR <<= 1;
			}
		}
	}

public:
	GPIO_Pin& PWRDWNCTL, & RST, & IOSYNC, & SDO, & CS, & SCLK, & SDIO, & OSK, & UPDATA;
	byte words0[4];// ��0�飺���ܿ�����
	byte words1[3];// ��1�飺ģ�ⲿ�ֿ�����  ����20M��Դ���룬20��Ƶ��VCO��Χ����Ϊ250M��400MHz�����໷�õ���Ϊ150UA
	byte ASF[2];// ��2��
	byte ARR[1];// ��3��
	byte FREQ[4];// ��4�飺Ƶ�ʿ�����
	byte PHASE[2];// ��5�飺��λ������ ��λƫ����
	AD9954(GPIO_Pin& PWRDWNCTL, GPIO_Pin& RST, GPIO_Pin& IOSYNC, GPIO_Pin& SDO, GPIO_Pin& CS, GPIO_Pin& SCLK, GPIO_Pin& SDIO, GPIO_Pin& OSK, GPIO_Pin& UPDATA) :
		PWRDWNCTL(PWRDWNCTL), RST(RST), IOSYNC(IOSYNC), SDO(SDO), CS(CS), SCLK(SCLK), SDIO(SDIO), OSK(OSK), UPDATA(UPDATA)
	{
		PWRDWNCTL = 0;
		RST = 0;
		IOSYNC = 0;
		SDO = 0;
		CS = 0;
		SCLK = 0;
		SDIO = 0;
		OSK = 0;
		UPDATA = 0;
		//
		words0[0] = 0x00; words0[1] = 0x10; words0[2] = 0x00; words0[3] = 0x00;
		words1[0] = 0xA7; words1[1] = 0x00; words1[2] = 0x00;
		ASF[0] = 0x00; ASF[1] = 0x00;
		ARR[0] = 0x00;
		FREQ[0] = 0x0A; FREQ[1] = 0xD7; FREQ[2] = 0xA3; FREQ[3] = 0x00;
		PHASE[0] = 0x00; PHASE[1] = 0x00;
	}

	void setMode(void) {
		GPIO_Pin* sbits[] = {
			&PWRDWNCTL,&RST,&IOSYNC,&SDO,&CS,&SCLK,&SDIO,&OSK,&UPDATA
		};
		for0(i, numsof(sbits))
			sbits[i]->setMode(GPIOMode::OUT_PushPull);
		//��ʼ��AD9952ʹ���Ϊ1MHZ
		PWRDWNCTL = 0;
		CS = 0;
		UPDATA = 0;
		IOSYNC = 0;
		RST = 1;
		SysDelay(5);
		RST = 0;
		Send(0X00, words0, 4);
		Send(0X01, words1, 3);
		Send(0X02, ASF, 2);
		Send(0X03, ARR, 1);
		Send(0X04, FREQ, 4);
		Send(0X05, PHASE, 2);
		UPDATA = 1;
		UPDATA.Toggle();
	}

	void setAmplitude(stduint Ampli)
	{
		unsigned char buf[] = { 0x40,0x20,0x00,0x02 }; // {0x02,0x00,0x20,0x40} ��
		// 2000 for 75, 4000 for 145mV 40000 for 250mV 0xFFFF for 570mV
		CS = 0;
		Send(0x00, buf, 4);
		ASF[0] = (byte)Ampli;
		ASF[1] = (byte)(Ampli >> 8) & 0x3f;
		Send(0x02, ASF, 2);// (0x03,ControlWord+7,1)��
		CS.Toggle();
		UPDATA = 1;
		UPDATA.Toggle();
	}

	void setFrequency(uint32 Freq)
	{
		uint32 Temp;
		Temp = (uint32)Freq * 10.73741824;	   //������Ƶ�����ӷ�Ϊ�ĸ��ֽ�  10.73741824=(2^32)/400000000
		FREQ[0] = (byte)Temp;
		FREQ[1] = (byte)(Temp >> 8);
		FREQ[2] = (byte)(Temp >> 16);
		FREQ[3] = (byte)(Temp >> 24);
		Send(0X04, FREQ, 4);
		Send(0X05, PHASE, 2);
		UPDATA = 1;
		UPDATA.Toggle();
	}

	// AD��λ�ǳ�ʼ��λ���������� 180-deg diff
	void setPhase(stduint ph)
	{
		CS = 0;
		PHASE[0] = (byte)ph;
		PHASE[1] = (byte)(ph >> 8) & 0x3f;
		Send(0x05, PHASE, 2);
		CS.Toggle();
		UPDATA = 1;
		UPDATA.Toggle();
	}

};

sbit &PWRDWNCTL = GPIOB[9];
sbit &RST = GPIOF[0];
sbit &IOSYNC  = GPIOB[6];
sbit &SDO = GPIOG[9];
sbit &CS = GPIOF[10];
sbit &SCLK = GPIOC[3];
sbit &SDIO = GPIOH[3];
sbit &OSK = GPIOA[6];
sbit &UPDATA = GPIOH[2];

int main() {
	RCC.setClock(SysclkSource::HSE);
	GPIO_Pin& LEDR = GPIOI[5];
	LEDR.setMode(GPIOMode::OUT_PushPull);

	AD9954 my_ad(PWRDWNCTL, RST, IOSYNC, SDO, CS, SCLK, SDIO, OSK, UPDATA);
	my_ad.setMode();
	my_ad.setFrequency(100.0e0);
	while (true)
	{
		my_ad.setPhase(0);
		SysDelay(50);
		my_ad.setPhase(0x4000 >> 1);
		SysDelay(50);
	}
}

