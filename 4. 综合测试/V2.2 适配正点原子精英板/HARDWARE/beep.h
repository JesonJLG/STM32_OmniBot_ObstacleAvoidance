#ifndef __BEEP_H
#define __BEEP_H
#include "sys.h"

#define BEEP_PORT	GPIOD
#define BEEP_DIO	GPIO_Pin_0

#define BEEP PDout(0)	// BEEP,�������ӿ�		   

void BEEP_Init(void);	//��ʼ��
void BEEP_Tick(void);	//��һ��
void BEEP_2Tick(void);	//������
void BEEP_3Tick(void);	//������
void BEEP_Ring(void);	//��һ��

#endif

