#ifndef __DHT11_H
#define __DHT11_H
#include "stm32f10x.h"                  // Device header

#define DHT11_RCC   RCC_APB2Periph_GPIOC
#define DHT11_PORT  GPIOC
#define DHT11_IO    GPIO_Pin_14


void DHT11_IO_OUT(void);    //温湿度模块输出函数
void DHT11_IO_IN(void);     //温湿度模块输入函数
void DHT11_RST(void);       //DHT11端口复位，发出起始信号（IO发送）
u8 DHT11_Check(void); 	    //等待DHT11回应，返回1:未检测到DHT11，返回0:成功（IO接收）	 
u8 DHT11_Init(void);	    //DHT11初始化
u8 DHT11_Read_Bit(void);
u8 DHT11_Read_Byte(void);
u8 DHT11_Read_Data(u8* temp, u8* humi);


#endif

