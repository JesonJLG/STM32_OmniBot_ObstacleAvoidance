#include "spi.h"


u8 SPI1_ReceiveData[10];
u8 SPI2_ReceiveData[10];

/**************************************************
�������ƣ�SPI1_Init()
�������ܣ�SPI1��ʼ�����������ó�����ģʽ
��ڲ�������
���ز�������
***************************************************/
void SPI1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;  
  SPI_InitTypeDef  SPI_InitStruct;   
  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );// GPIOAʱ��ʹ��,ѡ��SPI1����ӦPA4��PA5��PA6��PA7 
  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );// SPI1ʱ��ʹ�� 
	
	
  // ��ʼ��GPIOA��PA5/6/7�����ø����������AF_PP
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;           // �����������AF_PP
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);  // PA5/6/7�øߵ�ƽ
	
	
  // ��ʼ��SPI����
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// ����SPI�����˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;		// SPI1��Ϊ����
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;// ���SPI_CR1�Ĵ�����DFFλ����������֡��СΪ8λ
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;		  // ���SPI_CR1�Ĵ�����CPOLλ������ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;		  // ���SPI_CR1�Ĵ�����CPHAλ������ͬ��ʱ�ӵĵڶ��������أ��������أ����ݱ�����
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;			  // ���SPI_CR1�Ĵ�����SSMλ��NSS�ź��������ʹ��SSIλ������
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;// ���SPI_CR1�Ĵ�����BRλ��������Ԥ��ƵֵΪ256���������	
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// ���SPI_CR1�Ĵ�����LSBFIRSTλ�����ݴ����MSBλ��ʼ
  SPI_InitStruct.SPI_CRCPolynomial = 7;		      // ���SPI_CRCPR�Ĵ�����CRCPOLYλ����Ϊ0x0007��Ϊ��λֵ
  SPI_Init(SPI1, &SPI_InitStruct);  
  SPI_Cmd(SPI1, ENABLE);                        // ʹ��SPI����  
	
} 


/**************************************************
�������ƣ�u8 SPI1_ReadWriteByte(u8 TxData)
�������ܣ�SPI1��дһ���ֽں���
��ڲ�����TxData:Ҫд����ֽ�
���ز�������ȡ�����ֽ�
***************************************************/
u8 SPI1_ReadWriteByte(u8 TxData)
{		
  u8 retry=0;	
  // ���SPI_SR�Ĵ�����TXEλ�����ͻ���Ϊ�գ�����ֵ0ʱΪ�ǿգ�1ʱΪ��	
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) 
  {
	  retry++;                           // ���ͻ���Ϊ��ʱ��retry++
	  if(retry>200)return 0;
  }			  
  SPI_I2S_SendData(SPI1, TxData);       // ͨ������SPI2����һ������
  retry=0;
  // ���SPI_SR�Ĵ�����RXNEλ�����ջ���Ϊ�գ�����ֵ0ʱΪ�գ�1ʱΪ�ǿ�
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) 
  {
	   retry++;                         // �����ջ���Ϊ�ǿ�ʱ��retry++
	   if(retry>200)return 0;
  }	  						    
  return SPI_I2S_ReceiveData(SPI1);     // ����ͨ��SPIx������յ�����					    
}


/**************************************************
�������ƣ�SPI2_Init()
�������ܣ�SPI2��ʼ�����������óɴӻ�ģʽ
��ڲ�������
���ز�������
***************************************************/
void SPI2_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;  
  SPI_InitTypeDef  SPI_InitStruct;   
  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );     // GPIOBʱ��ʹ��,ѡ��SPI2����ӦPB12��PB13��PB14��PB15 
  RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );     // SPI2ʱ��ʹ�� 
	
  // ��ʼ��GPIOB��PB13/14/15�����ø����������AF_PP��PB14��ӦMISO�������Ϊ����������
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;                  // �����������AF_PP
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);      // PB13/14/15�øߵ�ƽ
	
  // ��ʼ��SPI����
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// ����SPI�����˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
  SPI_InitStruct.SPI_Mode = SPI_Mode_Slave;		  // SPI2��Ϊ�ӻ�
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;// ���SPI_CR1�Ĵ�����DFFλ����������֡��СΪ8λ
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;		  // ���SPI_CR1�Ĵ�����CPOLλ������ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;		  // ���SPI_CR1�Ĵ�����CPHAλ������ͬ��ʱ�ӵĵڶ��������أ��������أ����ݱ�����
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;			  // ���SPI_CR1�Ĵ�����SSMλ��NSS�ź��������ʹ��SSIλ������
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;// ���SPI_CR1�Ĵ�����BRλ��������Ԥ��ƵֵΪ256���������
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// ���SPI_CR1�Ĵ�����LSBFIRSTλ�����ݴ����MSBλ��ʼ
  SPI_InitStruct.SPI_CRCPolynomial = 7;		      // ���SPI_CRCPR�Ĵ�����CRCPOLYλ����Ϊ0x0007��Ϊ��λֵ
  SPI_Init(SPI2, &SPI_InitStruct);  
  SPI_Cmd(SPI2, ENABLE);                        // ʹ��SPI����   
	
} 


/**************************************************
�������ƣ�u8 SPI1_ReadWriteByte(u8 TxData)
�������ܣ�SPI1��дһ���ֽں���
��ڲ�����TxData:Ҫд����ֽ�
���ز�������ȡ�����ֽ�
***************************************************/
u8 SPI2_ReadWriteByte(u8 TxData)
{		
  u8 retry=0;	
  // ���SPI_SR�Ĵ�����TXEλ�����ͻ���Ϊ�գ�����ֵ0ʱΪ�ǿգ�1ʱΪ��	
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 
  {
		retry++;                           // ���ͻ���Ϊ��ʱ��retry++
		if(retry>200)return 0;
  }			  
  SPI_I2S_SendData(SPI2, TxData);       // ͨ������SPI2����һ������
  retry=0;
  // ���SPI_SR�Ĵ�����RXNEλ�����ջ���Ϊ�գ�����ֵ0ʱΪ�գ�1ʱΪ�ǿ�
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) 
  {
		retry++;                           // �����ջ���Ϊ�ǿ�ʱ��retry++
		if(retry>200)return 0;
  }	  						    
  return SPI_I2S_ReceiveData(SPI2);     // ����ͨ��SPIx������յ�����					    
}


/***********************************************************************
�������ƣ�SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
�������ܣ�SPI1�ٶ����ú���  SPI�ٶ�=fAPB2/��Ƶϵ�� fAPB2ʱ��һ��Ϊ84Mhz
��ڲ�����SPI_BaudRatePrescaler  
					SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
���ز�������
************************************************************************/
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
} 

/**************************************************
�������ƣ�SPI_text(void)
�������ܣ�SPI���Ժ���
��ڲ�������
���ز�������
***************************************************/
void SPI_text(void)
{
	u8 sendData=1;
	u8 ReceiveData=0;
	NSS1=0;
	SPI1_ReadWriteByte(sendData);
	ReceiveData=SPI2_ReadWriteByte(sendData);
	NSS1=1;
	printf("ReceiveData=%d ",ReceiveData);
	printf("\n");
	delay_ms(1000);
}



