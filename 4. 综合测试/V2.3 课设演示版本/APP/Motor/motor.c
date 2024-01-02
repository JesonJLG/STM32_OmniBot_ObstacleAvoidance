#include "motor.h"


/**************************************************
函数名称：Motor_PWM_Init(u32 arr,u32 psc)
函数功能：电机PWM输出函数
入口参数：arr 重装载值    psc   预分频系数
返回参数：无
***************************************************/
void Motor_PWM_Init(u32 arr,u32 psc)
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
	
	
	TIM_InitStructure.TIM_Period=arr-1;                    //自动重装载寄存器周期的值        
	TIM_InitStructure.TIM_Prescaler=psc-1;                 //设置预分频值 
	TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;    //设置时钟分割:TDTS = Tck_tim
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM向上计数模式
	TIM_InitStructure.TIM_RepetitionCounter=0;           //重复计数的值
	
	TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
	TIM_Cmd(TIM2, ENABLE);                               //使能计数器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //初始化外设TIM2 OC1
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //初始化外设TIM2 OC2
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //初始化外设TIM2 OC3
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //初始化外设TIM2 OC4
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	
}



/**************************************************
函数名称：Motor_GPIO_Init(void)
函数功能：电机GPIO初始化函数
入口参数：无
返回参数：无
***************************************************/
void Motor_GPIO_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
}


/**************************************************
函数名称：STBY_Init(void)
函数功能：STBY初始化
入口参数：无
返回参数：无
***************************************************/
void STBY_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     //禁用JTAG
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_9);
}

/**************************************************
函数名称：Motor_Init(void)
函数功能：电机相关引脚初始化
入口参数：无
返回参数：无
***************************************************/
void Motor_Init(void)
{
	Motor_PWM_Init(500,72);
	Motor_GPIO_Init();
	STBY_Init();
}


/**************************************************
函数名称：forward(u16 speed)
函数功能：小车前进
入口参数：speed  0-500
返回参数：无
***************************************************/
void forward(u16 speed)
{
	TIM_SetCompare1(TIM2,500-speed);//R_AIN2:右下
	R_AIN2_ON;
	
	TIM_SetCompare2(TIM2,speed);//R_BIN2:左下
	R_BIN2_OFF;
	
	TIM_SetCompare3(TIM2,speed);//L_AIN2:右上
	L_AIN2_OFF;
	
	TIM_SetCompare4(TIM2,500-speed);//L_BIN2:左上
  L_BIN2_ON;
}


/**************************************************
函数名称：backward(u16 speed)
函数功能：小车后退
入口参数：speed  0-500
返回参数：无
***************************************************/
void backward(u16 speed)
{
	TIM_SetCompare1(TIM2,speed);//R_AIN2:右下
	R_AIN2_OFF;
	
	TIM_SetCompare2(TIM2,500-speed);//R_BIN2:左下
	R_BIN2_ON;
	
	TIM_SetCompare3(TIM2,500-speed);//L_AIN2:右上
	L_AIN2_ON;
	
	TIM_SetCompare4(TIM2,speed);//L_BIN2:左上
  L_BIN2_OFF;	
}


/**************************************************
函数名称：Left_Turn(u16 speed)
函数功能：小车左转
入口参数：speed  0-500
返回参数：无
***************************************************/
void Left_Turn(u16 speed)
{
	TIM_SetCompare1(TIM2,500-speed);//R_AIN2:右下
	R_AIN2_ON;
	
	TIM_SetCompare2(TIM2,500-speed);//R_BIN2:左下
	R_BIN2_ON;
	
	TIM_SetCompare3(TIM2,speed);//L_AIN2:右上
	L_AIN2_OFF;
	
	TIM_SetCompare4(TIM2,speed);//L_BIN2:左上
  L_BIN2_OFF;	
}


/**************************************************
函数名称：Right_Turn(u16 speed)
函数功能：小车右转
入口参数：speed  0-500
返回参数：无
***************************************************/
void Right_Turn(u16 speed)
{
	TIM_SetCompare1(TIM2,speed);//R_AIN2:右下
	R_AIN2_OFF;
	
	TIM_SetCompare2(TIM2,speed);//R_BIN2:左下
	R_BIN2_OFF;
	
	TIM_SetCompare3(TIM2,500-speed);//L_AIN2:右上
	L_AIN2_ON;
	
	TIM_SetCompare4(TIM2,500-speed);//L_BIN2:左上
  L_BIN2_ON;	
}


/************************************************************************
函数名称：Move(u16 Dir,u16 speed)
函数功能：小车平移
入口参数：Dir 平移方向(L_Move R_Move L_U_Move L_D_Move R_U_Move L_D_Move)
					方向 speed  0-500
返回参数：无
*********************************************************&&*************/
void Move(u16 Dir,u16 speed)
{
	if(Dir==0)//左移
	{
		TIM_SetCompare1(TIM2,speed);//R_AIN2:右下
		R_AIN2_OFF;
	
		TIM_SetCompare2(TIM2,speed);//R_BIN2:左下
		R_BIN2_OFF;
	
		TIM_SetCompare3(TIM2,speed);//L_AIN2:右上
		L_AIN2_OFF;
	
		TIM_SetCompare4(TIM2,speed);//L_BIN2:左上
		L_BIN2_OFF;
	}
	else if(Dir==1)//右移
	{
		TIM_SetCompare1(TIM2,500-speed);//R_AIN2:右下
		R_AIN2_ON;
	
		TIM_SetCompare2(TIM2,500-speed);//R_BIN2:左下
		R_BIN2_ON;
	
		TIM_SetCompare3(TIM2,500-speed);//L_AIN2:右上
		L_AIN2_ON;
	
		TIM_SetCompare4(TIM2,500-speed);//L_BIN2:左上
		L_BIN2_ON;
	}
	else if(Dir==2)//左上移动
	{
		TIM_SetCompare1(TIM2,500-speed);//R_AIN2:右下
		R_AIN2_OFF;
		
		TIM_SetCompare2(TIM2,speed);//R_BIN2:左下
		R_BIN2_OFF;
		
		TIM_SetCompare3(TIM2,speed);//L_AIN2:右上
		L_AIN2_OFF;
		
		TIM_SetCompare4(TIM2,500-speed);//L_BIN2:左上
		L_BIN2_OFF;
	}
	else if(Dir==3)//右上移动
	{
		TIM_SetCompare1(TIM2,500-speed);//R_AIN2:右下
		R_AIN2_ON;
		
		TIM_SetCompare2(TIM2,speed);//R_BIN2:左下
		R_BIN2_ON;
		
		TIM_SetCompare3(TIM2,speed);//L_AIN2:右上
		L_AIN2_ON;
		
		TIM_SetCompare4(TIM2,500-speed);//L_BIN2:左上
		L_BIN2_ON;
	}
	else if(Dir==4)//左下移动
	{
		TIM_SetCompare1(TIM2,500-speed);//R_AIN2:右下
		R_AIN2_OFF;
		
		TIM_SetCompare2(TIM2,500-speed);//R_BIN2:左下
		R_BIN2_ON;
		
		TIM_SetCompare3(TIM2,500-speed);//L_AIN2:右上
		L_AIN2_ON;
		
		TIM_SetCompare4(TIM2,500-speed);//L_BIN2:左上
		L_BIN2_OFF;
	}
	else if(Dir==5)//右下移动
	{
		TIM_SetCompare1(TIM2,speed);//R_AIN2:右下
		R_AIN2_OFF;
		
		TIM_SetCompare2(TIM2,speed);//R_BIN2:左下
		R_BIN2_ON;
		
		TIM_SetCompare3(TIM2,speed);//L_AIN2:右上
		L_AIN2_ON;
		
		TIM_SetCompare4(TIM2,speed);//L_BIN2:左上
		L_BIN2_OFF;
	}
}


/**************************************************
函数名称：Motion_State(u16 mode)
函数功能：小车关闭及打开
入口参数：mode (ON OFF)
返回参数：无
***************************************************/
void Motion_State(u16 mode)
{
	if(mode==6)
	{
		L_STBY_ON;
		R_STBY_ON;
		TIM_SetCompare1(TIM2,500);//R_AIN2:右下
		R_AIN2_ON;
		
		TIM_SetCompare2(TIM2,0);//R_BIN2:左下
		R_BIN2_OFF;
		
		TIM_SetCompare3(TIM2,0);//L_AIN2:右上
		L_AIN2_OFF;
		
		TIM_SetCompare4(TIM2,500);//L_BIN2:左上
		L_BIN2_ON;
	}
	else if(mode==7)
	{
		L_STBY_OFF;
		R_STBY_OFF;
	}
}

