#include "led.h"

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIO端口时钟

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(LED_PORT, &GPIO_InitStructure);
}

void LED_ON(uint16_t PIN)
{
	GPIO_SetBits(LED_PORT, PIN);
}

void LED_OFF(uint16_t PIN)
{
	GPIO_ResetBits(LED_PORT, PIN);
}



