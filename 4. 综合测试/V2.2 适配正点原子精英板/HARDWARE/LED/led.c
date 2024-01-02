#include "led.h"	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LED0_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(LED0_PORT, &GPIO_InitStructure);
    GPIO_SetBits(LED0_PORT, LED0_PIN);

    GPIO_InitStructure.GPIO_Pin = LED1_PIN;	    		 
    GPIO_Init(LED1_PORT, &GPIO_InitStructure);	  				
    GPIO_SetBits(LED1_PORT,LED1_PIN);		
}

