#include "exti.h"
#include "key.h"
#include "led.h"

/**************************************************
�������ƣ�EXTI4_init(void)
�������ܣ��ⲿ�ж�4��ʼ������
��ڲ�������
���ز�������
***************************************************/
void EXTI4_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	Key_Init();//���󰴼���ʼ��
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;						//�ж���4
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;		//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;//�����ش���
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;          		//ʹ���ж�
	
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                        //��ʼ��NVIC�Ĵ���
	
}


/**************************************************
�������ƣ�EXTI4_IRQHandler(void)
�������ܣ��ⲿ�ж�4����
��ڲ�������
���ز�������
***************************************************/
void EXTI4_IRQHandler(void)//�жϷ�����
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
		LED=~LED;
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

