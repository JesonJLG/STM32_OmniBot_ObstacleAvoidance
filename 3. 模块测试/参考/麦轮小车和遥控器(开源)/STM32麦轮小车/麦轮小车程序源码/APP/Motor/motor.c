#include "motor.h"


/**************************************************
�������ƣ�Motor_PWM_Init(u32 arr,u32 psc)
�������ܣ����PWM�������
��ڲ�����arr ��װ��ֵ    psc   Ԥ��Ƶϵ��
���ز�������
***************************************************/
void Motor_PWM_Init(u32 arr,u32 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);     //������ʱ��2ʱ��


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//Ƶ��50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);								//��ʼ��GPIOA


    TIM_InitStructure.TIM_Period=arr-1;                    //�Զ���װ�ؼĴ������ڵ�ֵ
    TIM_InitStructure.TIM_Prescaler=psc-1;                 //����Ԥ��Ƶֵ
    TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;    //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM���ϼ���ģʽ
    TIM_InitStructure.TIM_RepetitionCounter=0;           //�ظ�������ֵ

    TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
    TIM_Cmd(TIM2, ENABLE);                               //ʹ�ܼ�����

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //��ʼ������TIM2 OC1
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //��ʼ������TIM2 OC2
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //��ʼ������TIM2 OC3
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //��ʼ������TIM2 OC4
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���

}



/**************************************************
�������ƣ�Motor_GPIO_Init(void)
�������ܣ����GPIO��ʼ������
��ڲ�������
���ز�������
***************************************************/
void Motor_GPIO_Init(void)
{
    GPIO_InitTypeDef	GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//�������
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB,&GPIO_InitStruct);

}


/**************************************************
�������ƣ�STBY_Init(void)
�������ܣ�STBY��ʼ��
��ڲ�������
���ز�������
***************************************************/
void STBY_Init(void)
{
    GPIO_InitTypeDef	GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     //����JTAG

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//�������
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB,&GPIO_InitStruct);

    GPIO_ResetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_9);
}

/**************************************************
�������ƣ�Motor_Init(void)
�������ܣ����������ų�ʼ��
��ڲ�������
���ز�������
***************************************************/
void Motor_Init(void)
{
    Motor_PWM_Init(500,72);	//4�������ռһ��PWM����
    Motor_GPIO_Init();		//4���������һ�������������
    STBY_Init();			//2���������� �������
}


/**************************************************
�������ƣ�forward(u16 speed)
�������ܣ�С��ǰ��
��ڲ�����speed  0-500
���ز�������
***************************************************/
void forward(u16 speed)
{
    TIM_SetCompare1(TIM2,500-speed);	//TIM2_CH1->PA0		R_AIN1_PWM��speedԽ��CCRԽС��ռ�ձ�Խ����ΪPWM2ģʽ��
    R_AIN2_ON;							//R_AIN2:����

    TIM_SetCompare2(TIM2,speed);		//TIM2_CH2->PA1
    R_BIN2_OFF;							//R_BIN2:����

    TIM_SetCompare3(TIM2,speed);		//TIM2_CH3->PA2
    L_AIN2_OFF;							//L_AIN2:����

    TIM_SetCompare4(TIM2,500-speed);	//TIM2_CH4->PA3
    L_BIN2_ON;							//L_BIN2:����
}


/**************************************************
�������ƣ�backward(u16 speed)
�������ܣ�С������
��ڲ�����speed  0-500
���ز�������
***************************************************/
void backward(u16 speed)
{
    TIM_SetCompare1(TIM2,speed);//R_AIN2:����
    R_AIN2_OFF;

    TIM_SetCompare2(TIM2,500-speed);//R_BIN2:����
    R_BIN2_ON;

    TIM_SetCompare3(TIM2,500-speed);//L_AIN2:����
    L_AIN2_ON;

    TIM_SetCompare4(TIM2,speed);//L_BIN2:����
    L_BIN2_OFF;
}


/**************************************************
�������ƣ�Left_Turn(u16 speed)
�������ܣ�С����ת
��ڲ�����speed  0-500
���ز�������
***************************************************/
void Left_Turn(u16 speed)
{
    TIM_SetCompare1(TIM2,500-speed);//R_AIN2:����
    R_AIN2_ON;

    TIM_SetCompare2(TIM2,500-speed);//R_BIN2:����
    R_BIN2_ON;

    TIM_SetCompare3(TIM2,speed);//L_AIN2:����
    L_AIN2_OFF;

    TIM_SetCompare4(TIM2,speed);//L_BIN2:����
    L_BIN2_OFF;
}


/**************************************************
�������ƣ�Right_Turn(u16 speed)
�������ܣ�С����ת
��ڲ�����speed  0-500
���ز�������
***************************************************/
void Right_Turn(u16 speed)
{
    TIM_SetCompare1(TIM2,speed);//R_AIN2:����
    R_AIN2_OFF;

    TIM_SetCompare2(TIM2,speed);//R_BIN2:����
    R_BIN2_OFF;

    TIM_SetCompare3(TIM2,500-speed);//L_AIN2:����
    L_AIN2_ON;

    TIM_SetCompare4(TIM2,500-speed);//L_BIN2:����
    L_BIN2_ON;
}


/************************************************************************
�������ƣ�Move(u16 Dir,u16 speed)
�������ܣ�С��ƽ��
��ڲ�����Dir ƽ�Ʒ���(L_Move R_Move L_U_Move L_D_Move R_U_Move L_D_Move)
					���� speed  0-500
���ز�������
*********************************************************&&*************/
void Move(u16 Dir,u16 speed)
{
    if(Dir==0)//����
    {
        TIM_SetCompare1(TIM2,speed);//R_AIN2:����
        R_AIN2_OFF;

        TIM_SetCompare2(TIM2,speed);//R_BIN2:����
        R_BIN2_OFF;

        TIM_SetCompare3(TIM2,speed);//L_AIN2:����
        L_AIN2_OFF;

        TIM_SetCompare4(TIM2,speed);//L_BIN2:����
        L_BIN2_OFF;
    }
    else if(Dir==1)//����
    {
        TIM_SetCompare1(TIM2,500-speed);//R_AIN2:����
        R_AIN2_ON;

        TIM_SetCompare2(TIM2,500-speed);//R_BIN2:����
        R_BIN2_ON;

        TIM_SetCompare3(TIM2,500-speed);//L_AIN2:����
        L_AIN2_ON;

        TIM_SetCompare4(TIM2,500-speed);//L_BIN2:����
        L_BIN2_ON;
    }
    else if(Dir==2)//�����ƶ�
    {
        TIM_SetCompare1(TIM2,500-speed);//R_AIN2:����
        R_AIN2_OFF;

        TIM_SetCompare2(TIM2,speed);//R_BIN2:����
        R_BIN2_OFF;

        TIM_SetCompare3(TIM2,speed);//L_AIN2:����
        L_AIN2_OFF;

        TIM_SetCompare4(TIM2,500-speed);//L_BIN2:����
        L_BIN2_OFF;
    }
    else if(Dir==3)//�����ƶ�
    {
        TIM_SetCompare1(TIM2,500-speed);//R_AIN2:����
        R_AIN2_ON;

        TIM_SetCompare2(TIM2,speed);//R_BIN2:����
        R_BIN2_ON;

        TIM_SetCompare3(TIM2,speed);//L_AIN2:����
        L_AIN2_ON;

        TIM_SetCompare4(TIM2,500-speed);//L_BIN2:����
        L_BIN2_ON;
    }
    else if(Dir==4)//�����ƶ�
    {
        TIM_SetCompare1(TIM2,500-speed);//R_AIN2:����
        R_AIN2_OFF;

        TIM_SetCompare2(TIM2,500-speed);//R_BIN2:����
        R_BIN2_ON;

        TIM_SetCompare3(TIM2,500-speed);//L_AIN2:����
        L_AIN2_ON;

        TIM_SetCompare4(TIM2,500-speed);//L_BIN2:����
        L_BIN2_OFF;
    }
    else if(Dir==5)//�����ƶ�
    {
        TIM_SetCompare1(TIM2,speed);//R_AIN2:����
        R_AIN2_OFF;

        TIM_SetCompare2(TIM2,speed);//R_BIN2:����
        R_BIN2_ON;

        TIM_SetCompare3(TIM2,speed);//L_AIN2:����
        L_AIN2_ON;

        TIM_SetCompare4(TIM2,speed);//L_BIN2:����
        L_BIN2_OFF;
    }
}


/**************************************************
�������ƣ�Motion_State(u16 mode)
�������ܣ�С���رռ���
��ڲ�����mode (ON OFF)
���ز�������
***************************************************/
void Motion_State(u16 mode)
{
    if(mode==6)
    {
        L_STBY_ON;
        R_STBY_ON;
        TIM_SetCompare1(TIM2,500);//R_AIN2:����
        R_AIN2_ON;

        TIM_SetCompare2(TIM2,0);//R_BIN2:����
        R_BIN2_OFF;

        TIM_SetCompare3(TIM2,0);//L_AIN2:����
        L_AIN2_OFF;

        TIM_SetCompare4(TIM2,500);//L_BIN2:����
        L_BIN2_ON;
    }
    else if(mode==7)
    {
        L_STBY_OFF;
        R_STBY_OFF;
    }
}

