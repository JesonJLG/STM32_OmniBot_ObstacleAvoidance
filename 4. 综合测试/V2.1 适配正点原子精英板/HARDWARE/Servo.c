#include "servo.h"

/*************************************
��ģ�����������ܣ�
1. PWM������ƶ��
2. ����20ms�ĸ����жϲ���1s�Ķ�ʱ�жϣ����ڲ���
*************************************/
//int time;

void PWM_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = SERVO_PWM;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SERVO_PORT, &GPIO_InitStructure);

    TIM_InternalClockConfig(SERVO_TIM);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;//ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;//PSC 72M/72/20 000 = 1000/20 = 50Hz = 0.02s = 20ms
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(SERVO_TIM, &TIM_TimeBaseInitStructure);

    TIM_ClearFlag(SERVO_TIM, TIM_FLAG_Update);		//����������жϱ�־λ
    TIM_ITConfig(SERVO_TIM, TIM_IT_Update,ENABLE);	//�����ж��������
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
    TIM_OC1Init(SERVO_TIM, &TIM_OCInitStructure);	//���˰��죬ԭ������������ͨ���ˣ�

    TIM_Cmd(SERVO_TIM, ENABLE);				//TIMx->CR1 |= TIM_CR1_CEN
    TIM_CtrlPWMOutputs(SERVO_TIM, ENABLE);	//TIMx->BDTR |= TIM_BDTR_MOE	�߼���ʱ��һ����Ҫ©����䣡����
}

void PWM_SetCompare1(uint16_t Compare)
{
    TIM_SetCompare1(SERVO_TIM, Compare);
}

void Servo_Init(void)
{
    PWM_Init();
}


//0~180		180��Χ�����0
//500~2500	2000��Χ�����500
//�������ϣ����еȱ������ż���
void Servo_SetAngle(float Angle)
{
    PWM_SetCompare1(Angle / 180 * 2000 + 500);
}


void initSG90(void)
{
    TIM_Cmd(SERVO_TIM, ENABLE);
    Servo_SetAngle(90);
}
void sgMiddle(void)		//�������Ϊ90��
{
    Servo_SetAngle(90);
}
void sgLeft(void)		//�������Ϊ0��
{
    Servo_SetAngle(10);
}
void sgRight(void)		//�������Ϊ180��
{
    Servo_SetAngle(170);
}
