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
#include "servo.h"
#include "oled.h"
#include "countsensor.h"
#include "serial.h"

//extern unsigned int rec_data[4];
//extern u8 mode_flag;			//��ʼΪ0
//extern char Pwm_RxPacket[20];	//"P[...]\r\n"	Pwm_RxPacket[0]Ϊpwm_limit
u8 t;
u8 temperature, humidity;
int cnt;				//��������ⲿ�жϴ���
int girdCount = 20;		//���̸�����
float lap;				//����Ȧ��
float perimeter = 8.1;	//�����ܳ� 2.575cm * 3.14 �� 8.1cm
float distance;			//·��
float speed;			//�ٶ�
int time;
extern char Pwm_RxFlag;

void analyzeIRSensorData_1s(void);

int main(void)
{
	delay_init();	    	 //��ʱ������ʼ��	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	USART1_Init(115200);	//printf
	Serial_Init(9600);		//BT06
	Motor_Init();
	Hcsr04_Init();			//������
	CountSensor_Init();
	OLED_Init();
	BEEP_Init();
	BEEP_Ring();
	Servo_Init();
	Servo_SetAngle(90);
	OLED_ShowString(1, 1, "Temp:00");
	OLED_ShowString(2, 1, "Humi:00");
	OLED_ShowString(3, 1, "cnt:00");
	printf("123\r\n");
	
	
	while (DHT11_Init())	//DHT11��ʼ��
	{
		printf("DHT11 Error\r\n");
		delay_ms(200);
	}

	Wait_BT_CONNECTED(); 	//�ȴ��������ӳɹ�
	while (1) 
	{
		Control();
		analyzeIRSensorData_1s();
		if (t % 10 == 0)			//ÿ100ms��ȡһ��
		{
			DHT11_Read_Data(&temperature, &humidity);	//��ȡ��ʪ��ֵ
			cnt = CountSensor_Get();
			OLED_ShowNum(1, 6, temperature, 2);
			OLED_ShowNum(2, 6, humidity, 2);
			OLED_ShowNum(3, 6, cnt, 2);
//			printf("temperature:%d\t", temperature);
//			printf("humidity:%d\t", humidity);
//			printf("cnt:%d\r\n", cnt);
			Serial_Printf("*T%d*", temperature);
			Serial_Printf("*H%d*", humidity);
//			Serial_Printf("*D%d*", cnt);
		}
		delay_ms(10);
		t++;
		if (t == 20)
		{
			t = 0;
			LED0 = !LED0;	//LED������˸
		}
		
	}
}

void analyzeIRSensorData_1s(void) 
{
	if(time >= 50)//20ms*50 = 1000ms = 1s
	{
		time = 0;
		lap = (float)cnt / girdCount;	//����Ȧ
		speed = lap * perimeter;		//�ٶ�cm/s
		CountSensor_Clear();
//		Serial_Printf("*D%.2f*", speed);
		Serial_Printf("*D1*");			//test
	}
}

////TIM1�����жϺ���	20msһ��
void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		time ++;
//		analyzeIRSensorData_1s();
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
