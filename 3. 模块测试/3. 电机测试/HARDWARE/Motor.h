#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "stm32f10x.h"
/*
左后MotorA(A1,A2): PB1,PB0	TIM3_43
左前MotorB(B1,B2): PA7,PA6	TIM3_21
右前MotorC(C1,C2): PA3,PA2	TIM2_43（复用至PB11、PB10）
右后MotorD(D1,D2): PA1,PA0	TIM2_21（复用至）
*/

#define timMOTOR_AB		TIM3
#define timMOTOR_CD		TIM2
///左后电机A
#define pinMOTOR_A1		GPIO_Pin_1		//PB1
#define pinMOTOR_A2		GPIO_Pin_0		//PB0
///左前电机B
#define pinMOTOR_B1		GPIO_Pin_7		//PA7
#define pinMOTOR_B2		GPIO_Pin_6		//PA6
///右前电机C
#define pinMOTOR_C1		GPIO_Pin_11		//PB11
#define pinMOTOR_C2		GPIO_Pin_10		//PB10
///右后电机D
#define pinMOTOR_D1		GPIO_Pin_3		//PB3
#define pinMOTOR_D2		GPIO_Pin_15		//PA15


void Motor_Init(void);
void Car_Stop(void);
void Car_Forward(uint16_t Speed);
void Car_Backward(uint16_t Speed);
void Car_TurnLeft(uint16_t Speed);
void Car_TurnRight(uint16_t Speed);
void Car_TransLeft(uint16_t Speed);
void Car_TransRight(uint16_t Speed);
void MotorA_SetSpeed(uint8_t Dir, uint16_t Speed);
void MotorB_SetSpeed(uint8_t Dir, uint16_t Speed);
void MotorC_SetSpeed(uint8_t Dir, uint16_t Speed);
void MotorD_SetSpeed(uint8_t Dir, uint16_t Speed);
#endif

