#include "hcsr.h"

u16 msHcCount = 0;

/**************************************************
�������ƣ�Hcsr04_NVIC(void)
�������ܣ�NVIC����
��ڲ�������
���ز�������
***************************************************/
void Hcsr04_NVIC(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;
   //	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

   NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}


/**************************************************
�������ƣ�Hcsr04_Init(void)
�������ܣ�IO�ڳ�ʼ�� ��������ʼ��
��ڲ�������
���ز�������
***************************************************/
void Hcsr04_Init(void)
{
   RCC_APB2PeriphClockCmd(HCSR04_CLK, ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Pin = HCSR04_TRIG;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
   GPIO_ResetBits(HCSR04_PORT, HCSR04_TRIG);

   GPIO_InitStructure.GPIO_Pin = HCSR04_ECHO;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
   GPIO_ResetBits(HCSR04_PORT, HCSR04_ECHO);

   TIM_DeInit(TIM4);
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
   TIM_TimeBaseStructure.TIM_Period = (1000 - 1);
   TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

   TIM_ClearFlag(TIM4, TIM_FLAG_Update);
   TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
   Hcsr04_NVIC();

   TIM_Cmd(TIM4, DISABLE);
}

/**************************************************
�������ƣ�OpenrForHc()  
�������ܣ��򿪶�ʱ��4
��ڲ�������
���ز�������
***************************************************/
static void OpenTimerForHc()
{
   TIM_SetCounter(TIM4, 0);
   msHcCount = 0;
   TIM_Cmd(TIM4, ENABLE);
}


/**************************************************
�������ƣ�void CloserForHc()  
�������ܣ��رն�ʱ��4
��ڲ�������
���ز�������
***************************************************/
static void CloseTimerForHc()
{
   TIM_Cmd(TIM4, DISABLE);
}


/**************************************************
�������ƣ�TIM4_IRQr(void)   
�������ܣ���ʱ��4���ж�
��ڲ�������
���ز�������
***************************************************/
void TIM4_IRQHandler(void)
{
   if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
   {
      TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
      msHcCount++;
   }
}

/**************************************************
�������ƣ�GetEcher(void)  
�������ܣ���ȡ��ʱ��4������ֵ
��ڲ�������
���ز�����t:����ֵ
***************************************************/
u32 GetEchoTimer(void)
{
   u32 t = 0;
   t = msHcCount * 1000;
   t += TIM_GetCounter(TIM4);
   TIM4->CNT = 0;
   delay_ms(50);
   return t;
}


/**************************************************
�������ƣ�Hcsr04GetLength(void)
�������ܣ�ͨ����ʱ��4������ֵ�������
��ڲ�������
���ز�����lengthTemp:����CM
***************************************************/
float Hcsr04GetLength(void)
{
   u32 t = 0;
   int i = 0;
   float lengthTemp = 0;
   float sum = 0;
   while (i != 5)
   {
      TRIG_Send = 1;
      delay_us(20);
      TRIG_Send = 0;
      while (ECHO_Reci == 0);
      OpenTimerForHc();
      i = i + 1;
      while (ECHO_Reci == 1);
      CloseTimerForHc();
      t = GetEchoTimer();
      lengthTemp = ((float)t / 58.0);//cm
      sum = lengthTemp + sum;
   }
   lengthTemp = sum / 5.0;
   return lengthTemp;
}


/**************************************************
�������ƣ�Hcsr04_Text(void)
�������ܣ����Ժ���
��ڲ�������
���ز�������
***************************************************/
void Hcsr04_Text(void)
{
   float r;
   r = Hcsr04GetLength();
   printf("r=%.2f\n", r);
   delay_ms(1000);
}





