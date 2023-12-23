#ifndef __HCSR_H
#define __HCSR_H	 
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stdlib.h"


//������Ӳ���ӿڶ���
#define HCSR04_PORT     GPIOB
#define HCSR04_CLK      RCC_APB2Periph_GPIOB
#define HCSR04_TRIG     GPIO_Pin_10
#define HCSR04_ECHO     GPIO_Pin_11

#define TRIG_Send  PBout(10) 
#define ECHO_Reci  PBin(11)


void Hcsr04_NVIC(void);
void Hcsr04_Init(void);
static void OpenTimerForHc(void);//�򿪶�ʱ��4
static void CloseTimerForHc(void);//�رն�ʱ��4
u32 GetEchoTimer(void);//��ȡ��ʱ��4������ֵ
float Hcsr04GetLength(void);//ͨ����ʱ��4������ֵ�������
void Hcsr04_Text(void);
		 				    
#endif



