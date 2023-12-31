#ifndef __MOTOR_H
#define __MOTOR_H	 
#include "sys.h"
#include "delay.h"



#define L_AIN2_ON  	GPIO_SetBits(GPIOB,GPIO_Pin_5)    //输出高电平
#define L_AIN2_OFF 	GPIO_ResetBits(GPIOB,GPIO_Pin_5)  //输出低电平
#define R_AIN2_ON 	GPIO_SetBits(GPIOB,GPIO_Pin_8)    //输出高电平
#define R_AIN2_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_8)  //输出低电平


#define L_BIN2_ON 	GPIO_SetBits(GPIOB,GPIO_Pin_6)    //输出高电平
#define L_BIN2_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_6)  //输出低电平
#define R_BIN2_ON 	GPIO_SetBits(GPIOB,GPIO_Pin_7)    //输出高电平
#define R_BIN2_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_7)  //输出低电平


#define L_STBY_ON 	GPIO_SetBits(GPIOB,GPIO_Pin_4)    //输出高电平,电机工作
#define L_STBY_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_4)  //输出低电平,电机停止
#define R_STBY_ON 	GPIO_SetBits(GPIOB,GPIO_Pin_9)    //输出高电平,电机工作
#define R_STBY_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_9)  //输出低电平,电机停止

#define L_Move   0
#define R_Move   1
#define L_U_Move 2
#define R_U_Move 3
#define L_D_Move 4
#define R_D_Move 5
#define ON       6
#define OFF      7


void Motor_PWM_Init(u32 arr,u32 psc);
void Motor_GPIO_Init(void);
void STBY_Init(void);
void Motor_Init(void);


void forward(u16 speed);
void backward(u16 speed);
void Left_Turn(u16 speed);
void Right_Turn(u16 speed);
void Move(u16 Dir,u16 speed);
void Motion_State(u16 mode);
		 				    
#endif
