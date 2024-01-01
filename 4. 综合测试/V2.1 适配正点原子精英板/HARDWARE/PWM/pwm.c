#include "pwm.h"
#include "led.h"
#include "delay.h"


/**************************************************
函数名称：PWM1_Init(u32 arr,u32 psc)
函数功能：PWM1输出函数
入口参数：arr 重装载值    psc   预分频系数
返回参数：无
***************************************************/
void PWM1_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);     //开启定时器1时钟
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50ZMHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化GPIOA
	
	
	TIM_InitStructure.TIM_Period=arr;                    //自动重装载寄存器周期的值        
	TIM_InitStructure.TIM_Prescaler=psc;                 //设置预分频值 
	TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;    //设置时钟分割:TDTS = Tck_tim
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM向上计数模式
	TIM_InitStructure.TIM_RepetitionCounter=0;           //重复计数的值
	
	TIM_TimeBaseInit(TIM1,&TIM_InitStructure);
	TIM_Cmd(TIM1, ENABLE);                               //使能计数器
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //初始化外设TIM1 OC1
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //初始化外设TIM1 OC2
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //初始化外设TIM1 OC3
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //初始化外设TIM1 OC4
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR2上的预装载寄存器
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}

/**************************************************
函数名称：PWM2_Init(u32 arr,u32 psc)
函数功能：PWM2输出函数
入口参数：arr 重装载值    psc   预分频系数
返回参数：无
***************************************************/
void PWM2_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);     //开启定时器2时钟
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50ZMHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化GPIOA
	
	
	TIM_InitStructure.TIM_Period=arr;                    //自动重装载寄存器周期的值        
	TIM_InitStructure.TIM_Prescaler=psc;                 //设置预分频值 
	TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;    //设置时钟分割:TDTS = Tck_tim
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM向上计数模式
	TIM_InitStructure.TIM_RepetitionCounter=0;           //重复计数的值
	
	TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
	TIM_Cmd(TIM2, ENABLE);                               //使能计数器
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //初始化外设TIM2 OC1
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //初始化外设TIM2 OC2
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //初始化外设TIM2 OC3
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //初始化外设TIM2 OC4
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	
	
}

/**************************************************
函数名称：PWM3_Init(u32 arr,u32 psc)
函数功能：PWM3输出函数
入口参数：arr 重装载值    psc   预分频系数
返回参数：无
***************************************************/
void PWM3_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);     //开启定时器3时钟
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50ZMHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化GPIOA
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50ZMHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);								//初始化GPIOB
	
	TIM_InitStructure.TIM_Period=arr;                    //自动重装载寄存器周期的值        
	TIM_InitStructure.TIM_Prescaler=psc;                 //设置预分频值 
	TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;    //设置时钟分割:TDTS = Tck_tim
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM向上计数模式
	TIM_InitStructure.TIM_RepetitionCounter=0;           //重复计数的值
	
	TIM_TimeBaseInit(TIM3,&TIM_InitStructure);
	TIM_Cmd(TIM3, ENABLE);                               //使能计数器
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //初始化外设TIM3 OC1
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //初始化外设TIM3 OC2
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //初始化外设TIM3 OC3
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //初始化外设TIM3 OC4
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	
	
}

/**************************************************
函数名称：PWM4_Init(u32 arr,u32 psc)
函数功能：PWM4输出函数
入口参数：arr 重装载值    psc   预分频系数
返回参数：无
***************************************************/
void PWM4_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);     //开启定时器4时钟
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50ZMHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);								//初始化GPIOB
	
	
	TIM_InitStructure.TIM_Period=arr;                    //自动重装载寄存器周期的值        
	TIM_InitStructure.TIM_Prescaler=psc;                 //设置预分频值 
	TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;    //设置时钟分割:TDTS = Tck_tim
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM向上计数模式
	TIM_InitStructure.TIM_RepetitionCounter=0;           //重复计数的值
	
	TIM_TimeBaseInit(TIM4,&TIM_InitStructure);
	TIM_Cmd(TIM4, ENABLE);                               //使能计数器
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //初始化外设TIM4 OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //初始化外设TIM4 OC2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //初始化外设TIM4 OC3
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //初始化外设TIM4 OC4
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	
	
}

/**************************************************
函数名称：PWM_Init(u32 arr,u32 psc)
函数功能：PWM初始化函数
入口参数：arr 重装载值    psc   预分频系数
返回参数：无
***************************************************/
void PWM_Init(u32 arr,u32 psc)
{
	PWM1_Init(arr,psc);
	PWM2_Init(arr,psc);
	PWM3_Init(arr,psc);
	PWM4_Init(arr,psc);
}


/**************************************************
函数名称：PWM(int angle)
函数功能：PWM信号转换角度函数
入口参数：angle  度数
返回参数：servo_temp 角度
***************************************************/
int PWM(int angle)
{
	u16 servo_temp;
	if(angle>180) angle=180;
	if(angle<0) angle=0;
	servo_temp=angle*200/180+50;
	servo_temp=2000-servo_temp;
	return servo_temp;
}

/**************************************************
函数名称：PWM_Text(void)
函数功能：PWM测试函数，用于驱动舵机
入口参数：无
返回参数：无
***************************************************/
void PWM_Text(void)
{
		TIM_SetCompare1(TIM1,PWM(90));
		TIM_SetCompare2(TIM1,PWM(90));
		TIM_SetCompare3(TIM1,PWM(90));
		TIM_SetCompare4(TIM1,PWM(90));
	  TIM_SetCompare1(TIM2,PWM(90));
		TIM_SetCompare2(TIM2,PWM(90));
		TIM_SetCompare3(TIM2,PWM(90));
		TIM_SetCompare4(TIM2,PWM(90));
		TIM_SetCompare1(TIM3,PWM(90));
		TIM_SetCompare2(TIM3,PWM(90));
		TIM_SetCompare3(TIM3,PWM(90));
		TIM_SetCompare4(TIM3,PWM(90));
		TIM_SetCompare1(TIM4,PWM(90));
		TIM_SetCompare2(TIM4,PWM(90));
		TIM_SetCompare3(TIM4,PWM(90));
		TIM_SetCompare4(TIM4,PWM(90));	
		delay_ms(1000);
	  TIM_SetCompare1(TIM1,PWM(170));
		TIM_SetCompare2(TIM1,PWM(170));
		TIM_SetCompare3(TIM1,PWM(170));
		TIM_SetCompare4(TIM1,PWM(170));
		TIM_SetCompare1(TIM2,PWM(170));
		TIM_SetCompare2(TIM2,PWM(170));
		TIM_SetCompare3(TIM2,PWM(170));
		TIM_SetCompare4(TIM2,PWM(170));
		TIM_SetCompare1(TIM3,PWM(170));
		TIM_SetCompare2(TIM3,PWM(170));
		TIM_SetCompare3(TIM3,PWM(170));
		TIM_SetCompare4(TIM3,PWM(170));
		TIM_SetCompare1(TIM4,PWM(170));
		TIM_SetCompare2(TIM4,PWM(170));
		TIM_SetCompare3(TIM4,PWM(170));
		TIM_SetCompare4(TIM4,PWM(170));
		delay_ms(1000);
		TIM_SetCompare1(TIM1,PWM(10));
		TIM_SetCompare2(TIM1,PWM(10));
		TIM_SetCompare3(TIM1,PWM(10));
		TIM_SetCompare4(TIM1,PWM(10));
		TIM_SetCompare1(TIM2,PWM(10));
		TIM_SetCompare2(TIM2,PWM(10));
		TIM_SetCompare3(TIM2,PWM(10));
		TIM_SetCompare4(TIM2,PWM(10));
		TIM_SetCompare1(TIM3,PWM(10));
		TIM_SetCompare2(TIM3,PWM(10));
		TIM_SetCompare3(TIM3,PWM(10));
		TIM_SetCompare4(TIM3,PWM(10));
		TIM_SetCompare1(TIM4,PWM(10));
		TIM_SetCompare2(TIM4,PWM(10));
		TIM_SetCompare3(TIM4,PWM(10));
		TIM_SetCompare4(TIM4,PWM(10));
		delay_ms(1000);
}


