/****************************************************************************

麦轮小车引脚分配

N20电机

STBY1:PB4    STBY2:PB9

1:PA2,PB6      2:PA3,PB5
3:PA0,PB8      4:PA1,PB7

蓝牙模块：usart2


NRF24L01

CE->PB13`  CSN->PB14  IRQ->PB15  SCK->PA5  MISO->PA6  MOSI->PA7

|-----------------|  IRQ   MISO
|       NRF     ::|  MOSI  SCK
|      24L01    ::|  CSN   CE
|-----------------|  VCC   GND


超声波模块

TRIG:PB10         ECHO:PB11

RGB彩灯

DI:PB0


************************************************************************************/
#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"


int main(void)
{
	delay_init();	    	 //延时函数初始化	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	USART1_Init(115200);	//printf
	USART2_Init(9600);		//BT06

	while (1)
	{
		//printf("test\r\n");
//		LED_ON(LED1_PIN);
//		LED_OFF(LED2_PIN);
//		delay_ms(1000);
//		LED_ON(LED2_PIN);
//		LED_OFF(LED1_PIN);
//		delay_ms(1000);
	}
}

