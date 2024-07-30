// ARMCC-5 GBK @dosconio
#include "MCU/ST/STM32F4"
using namespace uni;
GPIO_Pin& LEDR = GPIOI[5];

int main() {
	// ��system_stm32f4xx.c��SystemInit()������ϵͳʱ�ӳ�ʼ����16MHZ
	// ϵͳʱ�ӱ�׼����Ϊ168M�������216M�����Ƽ�
	// ���ݲ�֧�� HSI��
	// ���ݲ�ʹ�� SysTick ��ʱ��
	LEDR.setMode(GPIOMode::OUT_PushPull);
	for0 (i, 3*2) {
		LEDR.Toggle();
		for0(_i, 1000000);
	}
	if (!RCC.setClock(SysclkSource::HSE)) while (true);
	
	while (true) {
		LEDR.Toggle();
		for0(_i, 1000000);
	}
}
