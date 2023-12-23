#ifndef __SPI_H
#define __SPI_H	
#include "sys.h" 
#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"

#define NSS1 PAout(4)
#define NSS2 PBout(12)

void SPI1_Init(void);
void SPI2_Init(void);
u8 SPI1_ReadWriteByte(u8 TxData);
u8 SPI2_ReadWriteByte(u8 TxData);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);
void SPI_text(void);
	
#endif


