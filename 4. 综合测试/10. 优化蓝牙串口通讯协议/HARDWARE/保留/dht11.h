#ifndef __DHT11_H
#define __DHT11_H
#include "stm32f10x.h"                  // Device header

#define DHT11_RCC   RCC_APB2Periph_GPIOC
#define DHT11_PORT  GPIOC
#define DHT11_IO    GPIO_Pin_14


void DHT11_IO_OUT(void);    //��ʪ��ģ���������
void DHT11_IO_IN(void);     //��ʪ��ģ�����뺯��
void DHT11_RST(void);       //DHT11�˿ڸ�λ��������ʼ�źţ�IO���ͣ�
u8 DHT11_Check(void); 	    //�ȴ�DHT11��Ӧ������1:δ��⵽DHT11������0:�ɹ���IO���գ�	 
u8 DHT11_Init(void);	    //DHT11��ʼ��
u8 DHT11_Read_Bit(void);
u8 DHT11_Read_Byte(void);
u8 DHT11_Read_Data(u8* temp, u8* humi);


#endif

