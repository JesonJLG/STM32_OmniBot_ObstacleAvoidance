#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"                  // Device header

#define SERVO_PORT	GPIOA
#define SERVO_PWM	GPIO_Pin_8
#define SERVO_TIM	TIM1

#define MIDDLE 0
#define LIFT 1
#define RIGHT 2

void Servo_Init(void);
void Servo_SetAngle(float Angle);
void initSG90(void);
void sgMiddle(void);
void sgLeft(void);
void sgRight(void);

#endif
