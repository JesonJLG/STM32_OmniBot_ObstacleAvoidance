#include "hcsr.h"


u16 msHcCount = 0;


/**************************************************
函数名称：Hcsr04_NVIC(void)
函数功能：NVIC设置
入口参数：无
返回参数：无
***************************************************/
void Hcsr04_NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;             
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;       
	NVIC_Init(&NVIC_InitStructure);
}


/**************************************************
函数名称：Hcsr04_Init(void)
函数功能：IO口初始化 及其他初始化
入口参数：无
返回参数：无
***************************************************/
void Hcsr04_Init(void)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(HCSR04_CLK, ENABLE);
   
    GPIO_InitStructure.GPIO_Pin =HCSR04_TRIG;      
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
     
    GPIO_InitStructure.GPIO_Pin =   HCSR04_ECHO;     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
    GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);    
     
          
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   
     
    TIM_DeInit(TIM4);
    TIM_TimeBaseStructure.TIM_Period = (1000-1); 
    TIM_TimeBaseStructure.TIM_Prescaler =(72-1); 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);          


    TIM_ClearFlag(TIM4, TIM_FLAG_Update);  
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);    
    Hcsr04_NVIC();
    TIM_Cmd(TIM4,DISABLE);     
}

/**************************************************
函数名称：OpenTimerForHc()  
函数功能：打开定时器4
入口参数：无
返回参数：无
***************************************************/
static void OpenTimerForHc()  
{
   TIM_SetCounter(TIM4,0);
   msHcCount = 0;
   TIM_Cmd(TIM4, ENABLE); 
}


/**************************************************
函数名称：void CloseTimerForHc()  
函数功能：关闭定时器4
入口参数：无
返回参数：无
***************************************************/
static void CloseTimerForHc()    
{
   TIM_Cmd(TIM4, DISABLE); 
}


/**************************************************
函数名称：TIM4_IRQHandler(void)   
函数功能：定时器4终中断
入口参数：无
返回参数：无
***************************************************/
void TIM4_IRQHandler(void)  
{
   if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  
   {
       TIM_ClearITPendingBit(TIM4, TIM_IT_Update  ); 
       msHcCount++;
   }
}
 
/**************************************************
函数名称：GetEchoTimer(void)  
函数功能：获取定时器4计数器值
入口参数：无
返回参数：t:计数值
***************************************************/
u32 GetEchoTimer(void)
{
   u32 t = 0;
   t = msHcCount*1000;
   t += TIM_GetCounter(TIM4);
   TIM4->CNT = 0;  
   delay_ms(50);
   return t;
}


/**************************************************
函数名称：Hcsr04GetLength(void)
函数功能：通过定时器4计数器值推算距离
入口参数：无
返回参数：lengthTemp:距离CM
***************************************************/
float Hcsr04GetLength(void)
{
   u32 t = 0;
   int i = 0;
   float lengthTemp = 0;
   float sum = 0;
   while(i!=5)
   {
      TRIG_Send = 1;      
      delay_us(20);
      TRIG_Send = 0;
      while(ECHO_Reci == 0);   
      OpenTimerForHc();        
      i = i + 1;
      while(ECHO_Reci == 1);
      CloseTimerForHc();        
      t = GetEchoTimer();        
      lengthTemp = ((float)t/58.0);//cm
      sum = lengthTemp + sum ;     
   }
   lengthTemp = sum/5.0;
   return lengthTemp;
}


/**************************************************
函数名称：Hcsr04_Text(void)
函数功能：测试函数
入口参数：无
返回参数：无
***************************************************/
void Hcsr04_Text(void)
{
	float r;
	r=Hcsr04GetLength();
	printf("r=%.2f\n",r);
	delay_ms(1000);
}





