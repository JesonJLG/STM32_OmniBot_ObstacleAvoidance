#ifndef __SERIAL_H
#define __SERIAL_H
#include "stm32f10x.h"                  // Device header
#include <stdio.h>

#define USART_PORT	GPIOB
#define USART_TX	GPIO_Pin_10	//PB10
#define USART_RX	GPIO_Pin_11	//PB11
#define USARTX		USART3

void Serial_Init(u32 bound);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
void Wait_BT_CONNECTED(void);

#endif
