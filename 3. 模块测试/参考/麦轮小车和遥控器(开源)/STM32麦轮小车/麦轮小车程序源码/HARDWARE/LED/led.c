#include "led.h"
#include "delay.h"

/**************************************************
�������ƣ�LED_Init(void)
�������ܣ�LED��ʼ������
��ڲ�������
���ز�������
***************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����һ��GPIO_InitTypeDef���͵Ľṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//����GPIO�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13; //PC13
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//ͨ���������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//��������50MHZ
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}





