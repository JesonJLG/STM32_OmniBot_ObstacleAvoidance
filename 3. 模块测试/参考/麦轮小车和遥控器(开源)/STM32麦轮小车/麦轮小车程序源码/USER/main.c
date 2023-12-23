/****************************************************************************

����С�����ŷ���

N20���

STBY1:PB4    STBY2:PB9

1:PA2,PB6      2:PA3,PB5
3:PA0,PB8      4:PA1,PB7

����ģ�飺PA9,PA10


NRF24L01 

CE->PB13`  CSN->PB14  IRQ->PB15  SCK->PA5  MISO->PA6  MOSI->PA7  

|-----------------|  IRQ   MISO 
|       NRF     ::|  MOSI  SCK                       
|      24L01    ::|  CSN   CE
|-----------------|  VCC   GND


������ģ��

TRIG:PB10         ECHO:PB11

RGB�ʵ�

DI:PB0


************************************************************************************/


#include "stdio.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "control.h"


int main(void)
{	
	delay_init();	    	 //��ʱ������ʼ��	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init(); 
	USART1_Init(9600);	 //���ڳ�ʼ��Ϊ9600
	NRF24L01_Init();
  NRF24L01_Check_detection();//NRF24L01�ȴ�Ӧ��
	Motor_Init();//�����ʼ��
	Hcsr04_Init();//��������ʼ��
	Motion_State(OFF);//�رյ������ʧ��
	RGB_LED_Init();//RGB�ʵƳ�ʼ��
	delay_ms(1000);
	LED=0;
	while(1)
  {
		Control();	
	}
} 

