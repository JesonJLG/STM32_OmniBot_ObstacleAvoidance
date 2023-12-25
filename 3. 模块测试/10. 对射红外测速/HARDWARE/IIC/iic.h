#ifndef __IIC_H
#define __IIC_H	
#include "sys.h" 

#include "stm32f10x.h"
#include "delay.h"

#define  SCL_UP          GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define  SCL_DOWN     GPIO_ResetBits(GPIOB,GPIO_Pin_6)

#define  SDA_UP         GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define  SDA_DOWN    GPIO_ResetBits(GPIOB,GPIO_Pin_7)

#define   SDA_READ     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)//¶ÁÈ¡IOµçÆ½×´Ì¬


void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Nack(void);
void IIC_Ack(void);
void IIC_Send_Byte(u8 Byte);
u8 IIC_Read_Byte(void);

#endif

	



