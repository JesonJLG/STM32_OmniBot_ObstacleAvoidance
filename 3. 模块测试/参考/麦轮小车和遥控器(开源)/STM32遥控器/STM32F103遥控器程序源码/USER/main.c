/****************************************************************************

ң�������ŷ���

ҡ�ˣ�PA0��PA1��PA2��PA3

����ģ�飺PA9,PA10


NRF24L01

CE->PB13`  CSN->PB14  IRQ->PB15  SCK->PA5  MISO->PA6  MOSI->PA7

|-----------------|  IRQ   MISO
|       NRF     ::|  MOSI  SCK
|      24L01    ::|  CSN   CE
|-----------------|  VCC   GND


MPU6050ģ��

SCL:PB8         SDA:PB9

OLEDģ��

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

	delay_init();	    	 //��ʱ������ʼ��	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	USART1_Init(9600);	 //���ڳ�ʼ��Ϊ9600
	TIM3_Init(1999, 7199);//����̰����
	OLED_Init();
	MPU6050_Init();//�����ǳ�ʼ��
	JoyStick_Init();//ҡ�˳�ʼ��
	NRF24L01_Init();//NRFģ���ʼ��
	NRF24L01_Check_detection();//NRFӦ��

	LED = 0;
	while (1)
	{
		OLED_Control();
	}
}

