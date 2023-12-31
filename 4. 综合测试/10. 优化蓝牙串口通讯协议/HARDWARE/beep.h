#ifndef __BEEP_H
#define __BEEP_H
#include "sys.h"

#define BEEP_PORT	GPIOC
#define BEEP_DIO	GPIO_Pin_14

#define BEEP PCout(14)	// BEEP,蜂鸣器接口		   

void BEEP_Init(void);	//初始化
void BEEP_Tick(void);	//响一次
void BEEP_2Tick(void);	//响两次
void BEEP_3Tick(void);	//响三次
void BEEP_Ring(void);	//响一阵

#endif

