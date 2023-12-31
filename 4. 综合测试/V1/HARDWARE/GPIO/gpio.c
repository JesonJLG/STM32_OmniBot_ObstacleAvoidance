#include "gpio.h"


void GIPO_init(void)
{
	GPIO_InitTypeDef GPIO_InitTStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitTStructure.GPIO_Pin = GPIO_Pin_13;	
	GPIO_InitTStructure.GPIO_Mode= GPIO_Mode_OUT;
	GPIO_InitTStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitTStructure);	
	
}

