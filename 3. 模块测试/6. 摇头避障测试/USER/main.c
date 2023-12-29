/****************************************************************************

麦轮小车引脚分配

N20电机

STBY1:PB4    STBY2:PB9

1:PA2,PB6      2:PA3,PB5
3:PA0,PB8      4:PA1,PB7

蓝牙模块	usart2
超声波模块	TRIG:PB10         ECHO:PB11

************************************************************************************/
#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "motor.h"
#include "control.h"
#include "hcsr.h"
#include "beep.h"
#include "servo.h"

extern u8 mode_flag;	//初始为0

void motor_test(void);

int main(void)
{
	delay_init();	    	 //延时函数初始化	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	USART1_Init(115200);	//printf
	USART2_Init(9600);		//BT06
	Motor_Init();
	Hcsr04_Init();			//超声波
	BEEP_Init();
	BEEP_Ring();
	Servo_Init();
	Servo_SetAngle(90);
	
	Wait_BT_CONNECTED();

	while (1)
	{
		//printf("11111\r\n");
		Control();
		//Hcsr04_Text();
	}
}

void motor_test(void)
{
	printf("电机测试\r\n");
	Car_Forward(500);
	delay_ms(1800);
	Car_Backward(500);
	delay_ms(1800);
	Car_TransLeft(500);
	delay_ms(1800);
	Car_TransRight(500);
	delay_ms(1800);
	Car_TurnLeft(500);
	delay_ms(1800);
	Car_Stop();
	delay_ms(1800);
	Car_TurnRight(500);
	delay_ms(1800);
}
