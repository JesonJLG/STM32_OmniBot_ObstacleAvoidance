#include "hcsr.h"

u16 msHcCount = 0;	//毫秒计数器

/**************************************************
函数名称：Hcsr04_NVIC(void)
函数功能：NVIC设置
入口参数：无
返回参数：无
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
函数名称：Hcsr04_Init(void)
函数功能：IO口初始化 及其他初始化
入口参数：无
返回参数：无
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
    TIM_TimeBaseStructure.TIM_Period = (1000 - 1);	//1000次	1us*1000 = 1ms
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);	//72M/72 = 1MHz 即1us
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    Hcsr04_NVIC();

    TIM_Cmd(TIM4, DISABLE);
}

/**************************************************
函数名称：OpenrForHc()
函数功能：打开定时器4
入口参数：无
返回参数：无
***************************************************/
static void OpenTimerForHc()
{
    TIM_SetCounter(TIM4, 0);	//清空内部计数器
    msHcCount = 0;			//清空毫秒计数器变量
    TIM_Cmd(TIM4, ENABLE);	//开启TIM4，开始计数
}


/**************************************************
函数名称：void CloserForHc()
函数功能：关闭定时器4
入口参数：无
返回参数：无
***************************************************/
static void CloseTimerForHc()
{
    TIM_Cmd(TIM4, DISABLE);	//关闭TIM4，停止计数
}


/**************************************************
函数名称：TIM4_IRQr(void)
函数功能：定时器4终中断
入口参数：无
返回参数：无
***************************************************/
void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)	//每次触发中断 经过1us*1000 = 1ms时间
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        msHcCount++;	//更新毫秒计数器
    }
}

/**************************************************
函数名称：GetEcher(void)
函数功能：获取定时器4计数器值
入口参数：无
返回参数：t:计数值
***************************************************/
u32 GetEchoTimer(void)
{
    u32 t = 0;
    t = msHcCount * 1000;		//1 msHcCount相当于定时器 1000次计数
    t += TIM_GetCounter(TIM4);
    TIM4->CNT = 0;				//内部计数器清0
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
    while (i != 5)
    {
        TRIG_Send = 1;
        delay_us(20);				//10us以上的脉冲触发信号(高电平)
        TRIG_Send = 0;
        while (ECHO_Reci == 0);	//该模块内部就会发出8个40kHz的周期电平并检测回波，ECHO_Reci=1即收到回波，退出while
        OpenTimerForHc();			//收到回波，立刻开始计时
        i = i + 1;
        while (ECHO_Reci == 1);	//等待回波结束
        CloseTimerForHc();		//结束停止定时器
        t = GetEchoTimer();		//获取TIM4内部计数器的值（每次计数耗时1us [72M/72]）
        lengthTemp = ((float)t / 58.0);//lengthTemp：cm	t：us	lengthTemp = 340m/s*100/1000 000 * t / 2 = t/58.83
        sum = lengthTemp + sum;	//5次求和
    }
    lengthTemp = sum / 5.0;		//测5次取平均值
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
    r = Hcsr04GetLength();
    printf("r=%.2fcm\n", r);
    delay_ms(1000);
}





