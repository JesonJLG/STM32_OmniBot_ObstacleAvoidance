#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "stm32f10x.h"
/*------------添加引脚宏定义-------------*/
/*
左后MotorA(A1,A2): PB1,PB0	TIM3_43
左前MotorB(B1,B2): PA7,PA6	TIM3_21
右前MotorC(C1,C2): PA3,PA2	TIM2_43
右后MotorD(D1,D2): PA1,PA0	TIM2_21
*/
#define MOTOR_PORT_A		GPIOB
#define MOTOR_PORT_BCD		GPIOA

#define MOTOR_TIM_AB		TIM3
#define MOTOR_TIM_CD		TIM2
///左后电机
#define MOTORA_A1		GPIO_Pin_1		//PB1
#define MOTORA_A2		GPIO_Pin_0		//PB0
///左前电机
#define MOTORB_B1		GPIO_Pin_7		//PA7
#define MOTORB_B2		GPIO_Pin_6		//PA6
///右前电机
#define MOTORC_C1		GPIO_Pin_3		//PA3
#define MOTORC_C2		GPIO_Pin_2		//PA2
///右后电机
#define MOTORD_D1		GPIO_Pin_1		//PA1
#define MOTORD_D2		GPIO_Pin_0		//PA0


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

