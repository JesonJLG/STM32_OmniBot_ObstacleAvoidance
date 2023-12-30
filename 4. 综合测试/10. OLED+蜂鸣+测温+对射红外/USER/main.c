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
#include "dht11.h"
#include "beep.h"
#include "servo.h"
#include "oled.h"
#include "countsensor.h"
#include "serial.h"

//extern unsigned int rec_data[4];
//extern u8 mode_flag;			//初始为0
//extern u8 Pwm_RxPacket[20];	//"P[...]\r\n"	Pwm_RxPacket[0]为pwm_limit
extern u8 Lr_RxFlag, Rr_RxFlag, Pwm_RxFlag, Pr_RxFlag;
u8 t;
u8 temperature, humidity;
u16 cnt;

void motor_test(void);

int main(void)
{
	delay_init();	    	 //延时函数初始化	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	USART1_Init(115200);	//printf
	Serial_Init(9600);		//BT06
	Motor_Init();
	Hcsr04_Init();			//超声波
	CountSensor_Init();
	OLED_Init();
	BEEP_Init();
	BEEP_Ring();
	Servo_Init();
	Servo_SetAngle(90);
	OLED_ShowString(1, 1, "Temp:");
	OLED_ShowString(2, 1, "Humi:");
	OLED_ShowString(3, 1, "cnt:");
	
	
	while (DHT11_Init())	//DHT11初始化
	{
		printf("DHT11 Error\r\n");
		delay_ms(200);
	}

	Wait_BT_CONNECTED(); 
	while (1) 
	{
		Control();
		if (t % 10 == 0)			//每100ms读取一次
		{
			DHT11_Read_Data(&temperature, &humidity);	//读取温湿度值
			cnt = CountSensor_Get();
			OLED_ShowNum(1, 6, temperature, 2);
			OLED_ShowNum(2, 6, humidity, 2);
			OLED_ShowNum(3, 6, cnt, 4);
//			printf("temperature:%d\t", temperature);
//			printf("humidity:%d\t", humidity);
//			printf("cnt:%d\r\n", cnt);
			Serial_Printf("*T%d*", temperature);
			Serial_Printf("*H%d*", humidity);
			Serial_Printf("*D%d*", cnt);
		}
		delay_ms(10);
		t++;
		if (t == 20)
		{
			t = 0;
			LED0 = !LED0;
		}
		
		Pwm_RxFlag = 0;
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
