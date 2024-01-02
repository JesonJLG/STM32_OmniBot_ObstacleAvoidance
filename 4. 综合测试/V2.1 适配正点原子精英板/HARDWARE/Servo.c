#include "servo.h"

/*************************************
本模块兼具两个功能：
1. PWM输出控制舵机
2. 利用20ms的更新中断产生1s的定时中断，用于测速
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

    TIM_ClearFlag(SERVO_TIM, TIM_FLAG_Update);		//清除计数器中断标志位
    TIM_ITConfig(SERVO_TIM, TIM_IT_Update,ENABLE);	//配置中断输出控制
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
    TIM_OC1Init(SERVO_TIM, &TIM_OCInitStructure);	//搞了半天，原来是这里忘改通道了！

    TIM_Cmd(SERVO_TIM, ENABLE);				//TIMx->CR1 |= TIM_CR1_CEN
    TIM_CtrlPWMOutputs(SERVO_TIM, ENABLE);	//TIMx->BDTR |= TIM_BDTR_MOE	高级定时器一定不要漏了这句！！！
}

void PWM_SetCompare1(uint16_t Compare)
{
    TIM_SetCompare1(SERVO_TIM, Compare);
}

void Servo_Init(void)
{
    PWM_Init();
}


//0~180		180范围，起点0
//500~2500	2000范围，起点500
//根据以上，进行等比例缩放计算
void Servo_SetAngle(float Angle)
{
    PWM_SetCompare1(Angle / 180 * 2000 + 500);
}


void initSG90(void)
{
    TIM_Cmd(SERVO_TIM, ENABLE);
    Servo_SetAngle(90);
}
void sgMiddle(void)		//将舵机置为90度
{
    Servo_SetAngle(90);
}
void sgLeft(void)		//将舵机置为0度
{
    Servo_SetAngle(10);
}
void sgRight(void)		//将舵机置为180度
{
    Servo_SetAngle(170);
}
