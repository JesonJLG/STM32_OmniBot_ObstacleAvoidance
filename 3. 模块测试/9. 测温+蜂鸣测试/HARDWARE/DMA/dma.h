#ifndef __DMA_H
#define __DMA_H	
#include "sys.h" 

//初始化加上
//USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //使能串口1的DMA发送 

void DMA1_Init(void);
void USART1_DMA1_Init(void);
uint8_t Buffercmp(const uint32_t* pBuffer1,uint32_t* pBuffer2,uint32_t BufferSize);
void DMA_Text(void);
void USART_DMA_Text(void);
	

	
#endif


