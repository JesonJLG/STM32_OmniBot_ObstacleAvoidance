#include "led.h"
#include "delay.h"

/**************************************************
函数名称：LED_Init(void)
函数功能：LED初始化函数
入口参数：无
返回参数：无
***************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义一个GPIO_InitTypeDef类型的结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//开启GPIO端口时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13; //PC13
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//通用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//引脚速率50MHZ
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}





