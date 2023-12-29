#include "led.h"	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LED0_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(LED0_PORT, &GPIO_InitStructure);
    GPIO_SetBits(LED0_PORT, LED0_PIN);

    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 
    // GPIO_Init(GPIOE, &GPIO_InitStructure);	  				
    // GPIO_SetBits(GPIOE,GPIO_Pin_5); 					
}

