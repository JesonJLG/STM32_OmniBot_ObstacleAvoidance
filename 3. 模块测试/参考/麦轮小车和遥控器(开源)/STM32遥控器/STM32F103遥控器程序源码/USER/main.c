/****************************************************************************

遥控器引脚分配

摇杆：PA0、PA1、PA2、PA3

蓝牙模块：PA9,PA10


NRF24L01 

CE->PB13`  CSN->PB14  IRQ->PB15  SCK->PA5  MISO->PA6  MOSI->PA7  

|-----------------|  IRQ   MISO 
|       NRF     ::|  MOSI  SCK                       
|      24L01    ::|  CSN   CE
|-----------------|  VCC   GND


MPU6050模块

SCL:PB8         SDA:PB9

OLED模块

SCL:PA11         SDA:PA12


************************************************************************************/


#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "stdio.h"
#include "timer.h"
#include "oleddata.h"



int main(void)
{		
	 
	delay_init();	    	 //延时函数初始化	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init(); 
	USART1_Init(9600);	 //串口初始化为9600
	TIM3_Init(1999,7199);//用于贪吃蛇
	OLED_Init();
	MPU6050_Init();//陀螺仪初始化
	JoyStick_Init();//摇杆初始化
	NRF24L01_Init();//NRF模块初始化
  NRF24L01_Check_detection();//NRF应答
	
	LED=0;
	while(1)
  {
		OLED_Control();	
	}
} 

