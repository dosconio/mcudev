#include "key.h"
#include "delay.h"

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

u8 Get_Key(Key_Def key)
{
  	if ((KEY_PORT[key]->IDR&KEY_PIN[key])!=0)return 1;
  	else return 0;
}

u8 KEY_Scan(void)
{	 
	static u8 key_up=1;
	if(key_up&&(Get_Key(KEY1)==0||Get_Key(KEY2)==0||Get_Key(KEY3)==0||Get_Key(KEY4)==0))
	{
		Delay(0xFFFFF); 
		key_up=0;
		if(Get_Key(KEY1)==0)return 1;
		else if(Get_Key(KEY2)==0)return 2;
		else if(Get_Key(KEY3)==0)return 3;
		else if(Get_Key(KEY4)==0)return 4;
	}
	else if(Get_Key(KEY1)==1&&Get_Key(KEY2)==1&&Get_Key(KEY3)==1&&Get_Key(KEY4)==1)key_up=1; 	    
	return 0;
}



