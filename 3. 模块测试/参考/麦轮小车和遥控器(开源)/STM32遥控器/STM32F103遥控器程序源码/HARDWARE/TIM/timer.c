#include "timer.h"
#include "led.h"
#include "snake.h"

/*
��ʱʱ��=(arr+1)(psc+1)/72*10^6
*/

/**************************************************
�������ƣ�TIM2_Init(u32 arr,u32 psc)
�������ܣ���ʱ��2��ʼ������
��ڲ�����arr ��װ��ֵ    psc   Ԥ��Ƶϵ��
���ز�������
***************************************************/
void TIM2_Init(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);     //������ʱ��2ʱ��
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);				
	
	TIM_InitStructure.TIM_Period=arr;                    //�Զ���װ�ؼĴ������ڵ�ֵ        
	TIM_InitStructure.TIM_Prescaler=psc;                 //����Ԥ��Ƶֵ 
	TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;    //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_InitStructure.TIM_RepetitionCounter=0;           //�ظ�������ֵ
	
	TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);           //ʹ�ܼ������ж�
	
	TIM_Cmd(TIM2, ENABLE);                               //ʹ�ܼ�����
}


/**************************************************
�������ƣ�TIM3_Init(u32 arr,u32 psc)
�������ܣ���ʱ��3��ʼ������
��ڲ�����arr ��װ��ֵ    psc   Ԥ��Ƶϵ��
���ز�������
***************************************************/
void TIM3_Init(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);     //������ʱ��3ʱ��
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);				
	
	TIM_InitStructure.TIM_Period=arr;                    //�Զ���װ�ؼĴ������ڵ�ֵ        
	TIM_InitStructure.TIM_Prescaler=psc;                 //����Ԥ��Ƶֵ 
	TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;    //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_InitStructure.TIM_RepetitionCounter=0;           //�ظ�������ֵ
	
	TIM_TimeBaseInit(TIM3,&TIM_InitStructure);
	
	
	TIM_Cmd(TIM3, ENABLE);                               //ʹ�ܼ�����
}


/**************************************************
�������ƣ�TIM4_Init(u32 arr,u32 psc)
�������ܣ���ʱ��4��ʼ������
��ڲ�����arr ��װ��ֵ    psc   Ԥ��Ƶϵ��
���ز�������
***************************************************/
void TIM4_Init(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);     //������ʱ��4ʱ��
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);				
	
	TIM_InitStructure.TIM_Period=arr;                    //�Զ���װ�ؼĴ������ڵ�ֵ        
	TIM_InitStructure.TIM_Prescaler=psc;                 //����Ԥ��Ƶֵ 
	TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;    //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_InitStructure.TIM_RepetitionCounter=0;           //�ظ�������ֵ
	
	TIM_TimeBaseInit(TIM4,&TIM_InitStructure);
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);           //ʹ�ܼ������ж�
	
	TIM_Cmd(TIM4, ENABLE);                               //ʹ�ܼ�����
}


//TIM2�жϺ���
void TIM2_IRQHandler(void)                             
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET)      //�����жϱ�־λ
	{
		LED=~LED;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);        //�����־λ
	}
}

//TIM3�жϺ���
void TIM3_IRQHandler(void)                             
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET)      //�����жϱ�־λ
	{
		LED=~LED;
		Move();
		Eat_Food();
		drawSnake();
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);        //�����־λ
	}
}

//TIM4�жϺ���
void TIM4_IRQHandler(void)                             //TIM4�жϺ���
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update)!=RESET)      //�����жϱ�־λ
	{
		LED=~LED;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);        //�����־λ
	}
}

