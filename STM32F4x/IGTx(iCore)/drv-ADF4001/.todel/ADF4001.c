#include <stm32f10x.h>
#include "ADF4001.h"
#include "Delay.h"



//====================================================================================
// ��ʼ��������
//====================================================================================

//====================================================================================
// void GPIO_ADF4001_Configuration(void)
// ��������: ADF4001�������ú���
// ��ڲ���: ��
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: RCC_APB2PeriphClockCmd(); GPIO_Init();
// ע������: ��
//====================================================================================
void GPIO_ADF4001_Configuration(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;

	// ʹ��IO��ʱ��
	RCC_APB2PeriphClockCmd(  RCC_ADF4001_CE , ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = GPIO_ADF4001_Pin_CE; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			 // �������			 
	GPIO_Init(GPIO_ADF4001_Port_CE, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(  RCC_ADF4001_LE , ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = GPIO_ADF4001_Pin_LE; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			 // �������			 
	GPIO_Init(GPIO_ADF4001_Port_LE, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(  RCC_ADF4001_CLK , ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = GPIO_ADF4001_Pin_CLK; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			 // �������			 
	GPIO_Init(GPIO_ADF4001_Port_CLK, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(  RCC_ADF4001_DATA , ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = GPIO_ADF4001_Pin_DATA; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			 // �������			 
	GPIO_Init(GPIO_ADF4001_Port_DATA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(  RCC_ADF4001_MOUT , ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Pin = GPIO_ADF4001_Pin_MOUT;
	GPIO_Init(GPIO_ADF4001_Port_MOUT ,&GPIO_InitStructure);

}


void ADF4001_SetFre(u16 F100k,u32 lockmode)
{
	
	ADF4001_WR_Data(0x1f8093);			//��ʼ��
	Delay_ns(10);
	ADF4001_WR_Data(0x000000 | ADF4001LO<< 2 );	//30Mhz����������300��ƵΪ100Khz������λ��������������������������������������������������������������������������1
	Delay_ns(10);
	ADF4001_WR_Data(0x200001 | F100k<< 8 );	//100khz*1000��ƵΪ1Mhz
	Delay_ns(10);
	ADF4001_WR_Data(lockmode);	    //����ʱ�����
	
}

//====================================================================================
//��������:void ADF4001_Init(void)
//��������:ADF4001��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void ADF4001_Init(void)
{
	GPIO_ADF4001_Configuration();       
	Delay_1us(10)   ;
	
	ADF4001_CE_Set  ;
	Delay_1us(5)    ;
	ADF4001_LE_Clr  ;
	ADF4001_CLK_Clr ;
	ADF4001_DATA_Clr;	
	Delay_1us(1)    ;
	
	ADF4001_SetFre(1000,LockDetectMode_D);
	
}

//====================================================================================
// ADF4001������
//====================================================================================

//====================================================================================
//��������: void ADF4001_WR_Data(unsigned long int data)
//��������: ADF4001д������
//��ڲ���: data: 24bit ����														
//���ڲ���:��
//====================================================================================
void ADF4001_WR_Data(unsigned long int data)
{
	u8 i ;
	unsigned long int temp;
	
	temp = 0x800000;
// 	ADF4001_LE_Clr  ;
	ADF4001_CLK_Clr ;
	for( i=0; i< 24; i++)
	{
		if( data & temp )
			ADF4001_DATA_Set ;
		else 
			ADF4001_DATA_Clr ;
		temp >>= 1;	
		ADF4001_CLK_Set ;
		Delay_1ms(1);
		ADF4001_CLK_Clr ;
		Delay_1ms(1);
	}
	Delay_1ms(1);
	ADF4001_LE_Set ;
	Delay_1ms(1);
 	ADF4001_LE_Clr  ;
}



void TEST_ADF4001()
{
	u16 k = 1000;
	
	ADF4001_Init();	               					// ADF4001��ʼ��	
	
	while(1)
	{
		ADF4001_SetFre(k,LockDetectMode_D);		//�����������
		k = k+10;
		Delay_ms(1000);
		ADF4001_SetFre(k,LockDetectMode_A);		//ģ���������
		k = k+10;
		Delay_ms(1000);
	}
}

