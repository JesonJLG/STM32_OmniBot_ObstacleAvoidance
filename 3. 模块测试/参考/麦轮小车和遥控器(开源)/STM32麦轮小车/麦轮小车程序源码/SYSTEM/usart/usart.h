#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

//注意:使用蓝牙模块时波特率使用9600,不能超过9600波特率

void USART_SendByte(USART_TypeDef* USARTx,uint16_t Data);

void USART1_Init(u32 bound);
void USART2_Init(u32 bound);
void USART3_Init(u32 bound);

void USART1_Send_Byte(u8 Data);
void USART1_Send_nByte(u8 *Data, u16 size);
void USART1_Send_Str(u8 *Data);

void USART2_Send_Byte(u8 Data);
void USART2_Send_nByte(u8 *Data, u16 size);
void USART2_Send_Str(u8 *Data);

void USART3_Send_Byte(u8 Data);
void USART3_Send_nByte(u8 *Data, u16 size);
void USART3_Send_Str(u8 *Data);


#endif


