#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"                  // Device header

#define SERVO_PORT	GPIOA
#define SERVO_PWM	GPIO_Pin_8
#define SERVO_TIM	TIM1

void Servo_Init(void);
void Servo_SetAngle(float Angle);

#endif
