#include "exti.h"
#include "key.h"
#include "led.h"

/**************************************************
函数名称：EXTI4_init(void)
函数功能：外部中断4初始化函数
入口参数：无
返回参数：无
***************************************************/
void EXTI4_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	Key_Init();//矩阵按键初始化
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;						//中断线4
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;		//中断模式
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;//上升沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;          		//使能中断
	
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                        //初始化NVIC寄存器
	
}


/**************************************************
函数名称：EXTI4_IRQHandler(void)
函数功能：外部中断4函数
入口参数：无
返回参数：无
***************************************************/
void EXTI4_IRQHandler(void)//中断服务函数
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
		LED=~LED;
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

