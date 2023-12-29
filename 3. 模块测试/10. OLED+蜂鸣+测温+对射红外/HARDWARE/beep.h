#ifndef __BEEP_H
#define __BEEP_H
#include "sys.h"

#define BEEP_PORT	GPIOC
#define BEEP_DIO	GPIO_Pin_14

#define BEEP PCout(14)	// BEEP,�������ӿ�		   

void BEEP_Init(void);	//��ʼ��
void BEEP_Tick(void);	//��һ��
void BEEP_2Tick(void);	//������
void BEEP_3Tick(void);	//������
void BEEP_Ring(void);	//��һ��

#endif

