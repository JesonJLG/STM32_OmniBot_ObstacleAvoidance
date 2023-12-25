#include "beep.h"
#include "delay.h"

//蜂鸣器初始化
void BEEP_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC, GPIO_Pin_13);//输出1，关闭蜂鸣器输出
}

//响一次
void BEEP_Tick(void)
{
    BEEP = 0;   //响
    delay_ms(100);
    BEEP = 1;
    delay_ms(100);
}
//响两次
void BEEP_2Tick(void)
{
    BEEP_Tick();
    BEEP_Tick();
}
//响三次
void BEEP_3Tick(void)
{
    BEEP_Tick();
    BEEP_Tick();
    BEEP_Tick();
}
//响一阵
void BEEP_Ring(void)
{
    BEEP = 0;   //响
    delay_ms(200);
    BEEP = 1;
}
