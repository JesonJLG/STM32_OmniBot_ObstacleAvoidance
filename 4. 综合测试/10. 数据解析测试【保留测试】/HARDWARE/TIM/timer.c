#include "timer.h"
#include "led.h"

/*
定时时间=(arr+1)(psc+1)/72*10^6
*/

/**************************************************
函数名称：TIM2_Init(u32 arr,u32 psc)
函数功能：定时器2初始化函数
入口参数：arr 重装载值    psc   预分频系数
返回参数：无
***************************************************/
void TIM2_Init(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);     //开启定时器2时钟
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);				
	
	TIM_InitStructure.TIM_Period=arr;                    //自动重装载寄存器周期的值        
	TIM_InitStructure.TIM_Prescaler=psc;                 //设置预分频值 
	TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;    //设置时钟分割:TDTS = Tck_tim
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM向上计数模式
	TIM_InitStructure.TIM_RepetitionCounter=0;           //重复计数的值
	
	TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);           //使能计数器中断
	
	TIM_Cmd(TIM2, ENABLE);                               //使能计数器
}


/**************************************************
函数名称：TIM3_Init(u32 arr,u32 psc)
函数功能：定时器3初始化函数
入口参数：arr 重装载值    psc   预分频系数
返回参数：无
***************************************************/
void TIM3_Init(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);     //开启定时器3时钟
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);				
	
	TIM_InitStructure.TIM_Period=arr;                    //自动重装载寄存器周期的值        
	TIM_InitStructure.TIM_Prescaler=psc;                 //设置预分频值 
	TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;    //设置时钟分割:TDTS = Tck_tim
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM向上计数模式
	TIM_InitStructure.TIM_RepetitionCounter=0;           //重复计数的值
	
	TIM_TimeBaseInit(TIM3,&TIM_InitStructure);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);           //使能计数器中断
	
	TIM_Cmd(TIM3, ENABLE);                               //使能计数器
}


/**************************************************
函数名称：TIM4_Init(u32 arr,u32 psc)
函数功能：定时器4初始化函数
入口参数：arr 重装载值    psc   预分频系数
返回参数：无
***************************************************/
void TIM4_Init(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);     //开启定时器4时钟
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);				
	
	TIM_InitStructure.TIM_Period=arr;                    //自动重装载寄存器周期的值        
	TIM_InitStructure.TIM_Prescaler=psc;                 //设置预分频值 
	TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;    //设置时钟分割:TDTS = Tck_tim
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM向上计数模式
	TIM_InitStructure.TIM_RepetitionCounter=0;           //重复计数的值
	
	TIM_TimeBaseInit(TIM4,&TIM_InitStructure);
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);           //使能计数器中断
	
	TIM_Cmd(TIM4, ENABLE);                               //使能计数器
}


/*****************************************************************************

//TIM2中断函数
void TIM2_IRQHandler(void)                             
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET)      //接收中断标志位
	{
		LED=~LED;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);        //清除标志位
	}
}

//TIM3中断函数
void TIM3_IRQHandler(void)                             
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET)      //接收中断标志位
	{
		LED=~LED;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);        //清除标志位
	}
}

//TIM4中断函数
void TIM4_IRQHandler(void)                             //TIM4中断函数
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update)!=RESET)      //接收中断标志位
	{
		LED=~LED;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);        //清除标志位
	}
}

************************************************************************************/

