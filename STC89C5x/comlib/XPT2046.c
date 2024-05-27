#include <REGX51.H>
#include <INTRINS.H>

sbit XPY2046_DIN=P3^4;
sbit XPY2046_CS=P3^5;
sbit XPY2046_DCLK=P3^6;
sbit XPY2046_DOUT=P3^7;

/**
  * @brief  ZPT2046��ȡADֵ
  * @param  Command �����֣���Χ��ͷ�ļ��ڶ���ĺ꣬��β�����ֱ�ʾת����λ��
  * @retval ADת���������������Χ��8λΪ0~255��12λΪ0~4095
  */
unsigned int XPT2046ReadAD(unsigned char Command)
{
	// ret: 0~255 or 0~4095
	unsigned char i;
	unsigned int Data=0;
	XPY2046_DCLK=0;
	XPY2046_CS=0;
	for(i=0;i<8;i++)
	{
		XPY2046_DIN=Command&(0x80>>i);
		XPY2046_DCLK=1;
		XPY2046_DCLK=0;
	}
	for(i=0;i<16;i++)
	{
		XPY2046_DCLK=1;
		XPY2046_DCLK=0;
		if(XPY2046_DOUT){Data|=(0x8000>>i);}
	}
	XPY2046_CS=1;
	return Data>>8;
}
