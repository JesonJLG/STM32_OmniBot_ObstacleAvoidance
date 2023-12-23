#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"                  // Device header

void USART1_Init(u32 bound);
void USART2_Init(u32 bound);
void USART3_Init(u32 bound);
void USART_SendByte(USART_TypeDef* USARTx,uint16_t Data);

#endif


