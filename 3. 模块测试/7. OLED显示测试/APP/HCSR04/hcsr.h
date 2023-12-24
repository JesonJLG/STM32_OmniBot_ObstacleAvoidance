#ifndef __HCSR_H
#define __HCSR_H	 
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stdlib.h"


//超声波硬件接口定义
#define HCSR04_PORT     GPIOB
#define HCSR04_CLK      RCC_APB2Periph_GPIOB
#define HCSR04_TRIG     GPIO_Pin_10
#define HCSR04_ECHO     GPIO_Pin_11

#define TRIG_Send  PBout(10) 
#define ECHO_Reci  PBin(11)


void Hcsr04_NVIC(void);
void Hcsr04_Init(void);
static void OpenTimerForHc(void);//打开定时器4
static void CloseTimerForHc(void);//关闭定时器4
u32 GetEchoTimer(void);//获取定时器4计数器值
float Hcsr04GetLength(void);//通过定时器4计数器值推算距离
void Hcsr04_Text(void);
		 				    
#endif



