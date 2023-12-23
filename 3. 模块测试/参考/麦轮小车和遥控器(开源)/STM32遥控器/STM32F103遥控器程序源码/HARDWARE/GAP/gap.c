#include "gap.h"

u8  TIM2CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM2CH1_CAPTURE_VAL;	//输入捕获值
u32 temp=0; 


/**************************************************
函数名称：TIM2_Gap_Init(u32 arr,u32 psc)
函数功能：定时器2输入捕获初始化函数
入口参数：arr 重装载值    psc   预分频系数
返回参数：无
***************************************************/
void TIM2_Gap_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);     //开启定时器2时钟
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50ZMHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;				//输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化GPIOA
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);//下拉
	
	
	TIM_InitStructure.TIM_Period=arr;                    //自动重装载寄存器周期的值        
	TIM_InitStructure.TIM_Prescaler=psc;                 //设置预分频值 
	TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;    //设置时钟分割:TDTS = Tck_tim
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM向上计数模式
	TIM_InitStructure.TIM_RepetitionCounter=0;           //重复计数的值
	
	TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
	TIM_Cmd(TIM2, ENABLE);                               //使能计数器
	
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;//选择输入端，IC1映射到TI1上
	TIM_ICInitStructure.TIM_ICFilter=0x00;//配置滤波，IC1F=0000 不滤波
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//上升沿捕获
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//配置输入分频，不分频
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//映射到TI1上
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);

  TIM_ITConfig(TIM2, TIM_IT_Update|TIM_IT_CC1, ENABLE);   //允许更新中断，允许CC1TE捕获中断
	TIM_Cmd(TIM2,ENABLE);//使能定时器2
		
	
}



////定时器2中断服务程序	 
//void TIM2_IRQHandler(void)
//{ 

// 	if((TIM2CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
//	{	  
//		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//		 
//		{	    
//			if(TIM2CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
//			{
//				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
//				{
//					TIM2CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
//					TIM2CH1_CAPTURE_VAL=0XFFFF;
//				}else TIM2CH1_CAPTURE_STA++;
//			}	 
//		}
//	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
//		{	
//			if(TIM2CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
//			{	  			
//				TIM2CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
//				TIM2CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
//		   	TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
//			}else  								//还未开始,第一次捕获上升沿
//			{
//				TIM2CH1_CAPTURE_STA=0;			//清空
//				TIM2CH1_CAPTURE_VAL=0;
//	 			TIM_SetCounter(TIM2,0);
//				TIM2CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
//		   	TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
//			}		    
//		}			     	    					   
// 	} 
//  TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
//}


/**************************************************
函数名称：TIM2_Gap_text(void)
函数功能：定时器2输入捕获测试函数
入口参数：无
返回参数：无
***************************************************/
void TIM2_Gap_text(void)
{
		delay_ms(10);
		TIM_SetCompare2(TIM2,TIM_GetCapture2(TIM2)+1);

		if(TIM_GetCapture2(TIM2)==300)TIM_SetCompare2(TIM2,0);	
		 		 
 		if(TIM2CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			temp=TIM2CH1_CAPTURE_STA&0X3F;
			temp*=65536;//溢出时间总和
			temp+=TIM2CH1_CAPTURE_VAL;//得到总的高电平时间
			printf("HIGH:%d us\r\n",temp);//打印总的高点平时间
			TIM2CH1_CAPTURE_STA=0;//开启下一次捕获
		}
}


