#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"                  // Device header

#define portServo	GPIOA
#define pinServo	GPIO_Pin_8
#define timServo	TIM1

void Servo_Init(void);
void Servo_SetAngle(float Angle);

#endif
