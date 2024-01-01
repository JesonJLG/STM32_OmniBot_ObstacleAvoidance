#include "Motor.h"

void Motor_Init()
{
    /*------------RCC-------------*/
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

    /*------------AFIO-------------*/
//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//��ֹJTAG���ܣ���PB3��PB4��Ϊ��ͨIO��ʹ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//	DBGMCU->CR  &= ~((uint32_t)1<<5);
//	AFIO->MAPR = (AFIO->MAPR & ~((uint32_t)0x7 << 24)) | (2 << 24);     /*  PA15 PB3 PB4 */

    /*------------GPIO-------------*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = MOTORA_A1|MOTORA_A2;	//PB1��PB0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_PORT_A,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = MOTORB_B1|MOTORB_B2|MOTORC_C1|MOTORC_C2|MOTORD_D1|MOTORD_D2;	//PA0��PA1��PA2��PA3��PA6��PA7
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_PORT_BCD,&GPIO_InitStructure);

    /*------------TIM-------------*/
    TIM_InternalClockConfig(MOTOR_TIM_AB);
    TIM_InternalClockConfig(MOTOR_TIM_CD);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);			//��ʼ��Ĭ�Ͻṹ��
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 720-1;
    TIM_TimeBaseInitStructure.TIM_Period = 1000-1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(MOTOR_TIM_AB,&TIM_TimeBaseInitStructure);
    TIM_TimeBaseInit(MOTOR_TIM_CD,&TIM_TimeBaseInitStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);						//��ʼ��Ĭ�Ͻṹ��
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1000;    //CCR

    TIM_OC1Init(MOTOR_TIM_AB,&TIM_OCInitStructure);
    TIM_OC2Init(MOTOR_TIM_AB,&TIM_OCInitStructure);
    TIM_OC3Init(MOTOR_TIM_AB,&TIM_OCInitStructure);
    TIM_OC4Init(MOTOR_TIM_AB,&TIM_OCInitStructure);

    TIM_OC1Init(MOTOR_TIM_CD,&TIM_OCInitStructure);
    TIM_OC2Init(MOTOR_TIM_CD,&TIM_OCInitStructure);
    TIM_OC3Init(MOTOR_TIM_CD,&TIM_OCInitStructure);
    TIM_OC4Init(MOTOR_TIM_CD,&TIM_OCInitStructure);

    TIM_OC1PreloadConfig(MOTOR_TIM_AB, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_OC2PreloadConfig(MOTOR_TIM_AB, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_OC3PreloadConfig(MOTOR_TIM_AB, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_OC4PreloadConfig(MOTOR_TIM_AB, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���

    TIM_OC1PreloadConfig(MOTOR_TIM_CD, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(MOTOR_TIM_CD, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(MOTOR_TIM_CD, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(MOTOR_TIM_CD, TIM_OCPreload_Enable);

    TIM_Cmd(MOTOR_TIM_AB,ENABLE);
    TIM_Cmd(MOTOR_TIM_CD,ENABLE);

}

void Car_Stop()
{
    MotorA_SetSpeed(1,0);
    MotorB_SetSpeed(1,0);
    MotorC_SetSpeed(1,0);
    MotorD_SetSpeed(1,0);
}

void Car_Forward(uint16_t Speed)	//ǰ����1��1��1��1��
{
    MotorA_SetSpeed(1,Speed);
    MotorB_SetSpeed(1,Speed);
    MotorC_SetSpeed(1,Speed);
    MotorD_SetSpeed(1,Speed);
}

void Car_Backward(uint16_t Speed)	//���ˣ�0��0��0��0��
{
    MotorA_SetSpeed(0,Speed);
    MotorB_SetSpeed(0,Speed);
    MotorC_SetSpeed(0,Speed);
    MotorD_SetSpeed(0,Speed);
}

void Car_TurnLeft(uint16_t Speed)	//��ת��0��0��1��1��
{
    MotorA_SetSpeed(0,Speed);
    MotorB_SetSpeed(0,Speed);
    MotorC_SetSpeed(1,Speed);
    MotorD_SetSpeed(1,Speed);
}

void Car_TurnRight(uint16_t Speed)	//��ת��1��1��0��0��
{
    MotorA_SetSpeed(1,Speed);
    MotorB_SetSpeed(1,Speed);
    MotorC_SetSpeed(0,Speed);
    MotorD_SetSpeed(0,Speed);
}

void Car_TransLeft(uint16_t Speed)	//��ƽ�ƣ�1��0��1��0��
{
    MotorA_SetSpeed(1,Speed);
    MotorB_SetSpeed(0,Speed);
    MotorC_SetSpeed(1,Speed);
    MotorD_SetSpeed(0,Speed);
}

void Car_TransRight(uint16_t Speed)	//��ƽ�ƣ�0��1��0��1��
{
    MotorA_SetSpeed(0,Speed);
    MotorB_SetSpeed(1,Speed);
    MotorC_SetSpeed(0,Speed);
    MotorD_SetSpeed(1,Speed);
}

/*****************************************************/
//  PA0->TIM2_CH1->MOTOR1A->Backward
//  PA1->TIM2_CH2->MOTOR1B->Forward
//motor1 ��ת����Ϊ�Ӿ��ϵķ�ת ��Dir = 0;
/*------------MOTORA-------------*/
void MotorA_SetSpeed(uint8_t Dir, uint16_t Speed)
{
    if (Speed>=1000) Speed = 1000;
    if (Dir)
    {
        TIM_SetCompare4(MOTOR_TIM_AB,0);
        TIM_SetCompare3(MOTOR_TIM_AB,Speed);
    }
    else if (!Dir)
    {
        TIM_SetCompare3(MOTOR_TIM_AB,0);
        TIM_SetCompare4(MOTOR_TIM_AB,Speed);
    }
}

//  PA2->TIM2_CH3->MOTOR2A->Backward
//  PA3->TIM2_CH4->MOTOR2B->Forward
// motor2 ��ת����Ϊ�Ӿ��ϵ���ת ��DIR = 1��
/*------------MOTORB-------------*/
void MotorB_SetSpeed(uint8_t Dir, uint16_t Speed)
{
    if (Speed>=1000) Speed = 1000;
    if (Dir)
    {
        TIM_SetCompare2(MOTOR_TIM_AB,0);
        TIM_SetCompare1(MOTOR_TIM_AB,Speed);
    }
    else if (!Dir)
    {
        TIM_SetCompare1(MOTOR_TIM_AB,0);
        TIM_SetCompare2(MOTOR_TIM_AB,Speed);
    }
}

//  PB6->TIM4_CH1->MOTOTR3A(OUT4A)->Backward
//  PB7->TIM4_CH2->MOTOTR3B(OUT4B)->Forward
// motor3 ��ת����Ϊ�Ӿ��ϵ���ת ��DIR = 1��
/*------------MOTORC-------------*/
void MotorC_SetSpeed(uint8_t Dir, uint16_t Speed)
{
    if (Speed>=1000) Speed = 1000;
    if (Dir)
    {
        TIM_SetCompare3(MOTOR_TIM_CD,0);
        TIM_SetCompare4(MOTOR_TIM_CD,Speed);
    }
    else if (!Dir)
    {
        TIM_SetCompare4(MOTOR_TIM_CD,0);
        TIM_SetCompare3(MOTOR_TIM_CD,Speed);
    }
}

//  PB8->TIM4_CH3->MOTOR4A(OUT3A)->Backward
//  PB9->TIM4_CH4->MOTOR4B(OUT3B)->Forward
// MOtor4 ��ת���Ӿ��ϵķ�ת ��DIR  = 0��
/*------------MOTORD-------------*/
void MotorD_SetSpeed(uint8_t Dir, uint16_t Speed)
{
    if (Speed>=1000) Speed = 1000;
    if (Dir)
    {
        TIM_SetCompare1(MOTOR_TIM_CD,0);
        TIM_SetCompare2(MOTOR_TIM_CD,Speed);
    }
    else if (!Dir)
    {
        TIM_SetCompare2(MOTOR_TIM_CD,0);
        TIM_SetCompare1(MOTOR_TIM_CD,Speed);
    }
}
