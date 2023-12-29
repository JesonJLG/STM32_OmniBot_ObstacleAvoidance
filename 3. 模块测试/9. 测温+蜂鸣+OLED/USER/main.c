/****************************************************************************

����С�����ŷ���

N20���

STBY1:PB4    STBY2:PB9

1:PA2,PB6      2:PA3,PB5
3:PA0,PB8      4:PA1,PB7

����ģ��	usart2
������ģ��	TRIG:PB10         ECHO:PB11

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
#include "oled.h"

extern unsigned int rec_data[4];
extern u8 mode_flag;	//��ʼΪ0
u8 t;
u8 temperature, humidity;

void motor_test(void);

int main(void)
{
	delay_init();	    	 //��ʱ������ʼ��	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	USART1_Init(115200);	//printf
	USART2_Init(9600);		//BT06
	Motor_Init();
	Hcsr04_Init();			//������
	OLED_Init();
	BEEP_Init();
	BEEP_Ring();
	OLED_ShowString(1, 1, "Temp:");
	OLED_ShowString(2, 1, "Humi:");

	while (DHT11_Init())	//DHT11��ʼ��
	{
		printf("DHT11 Error\r\n");
		delay_ms(200);
	}

	while (1) 
	{
		if (t % 10 == 0)			//ÿ100ms��ȡһ��
		{
			DHT11_Read_Data(&temperature, &humidity);	//��ȡ��ʪ��ֵ
//            LCD_ShowNum(30+40,150,temperature,2,16);	//��ʾ�¶�
//            LCD_ShowNum(30+40,170,humidity,2,16);		//��ʾʪ��
			OLED_ShowNum(1, 6, temperature, 2);
			OLED_ShowNum(2, 6, humidity, 2);
			printf("temperature:%d\t", temperature);
			printf("humidity:%d\r\n", humidity);
		}
		delay_ms(10);
		t++;
		if (t == 20)
		{
			t = 0;
			LED0 = !LED0;
		}

	}
}

void motor_test(void)
{
	printf("�������\r\n");
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
