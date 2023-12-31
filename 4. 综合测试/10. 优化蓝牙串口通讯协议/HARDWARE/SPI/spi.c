#include "spi.h"


u8 SPI1_ReceiveData[10];
u8 SPI2_ReceiveData[10];

/**************************************************
函数名称：SPI1_Init()
函数功能：SPI1初始化函数：配置成主机模式
入口参数：无
返回参数：无
***************************************************/
void SPI1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;  
  SPI_InitTypeDef  SPI_InitStruct;   
  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );// GPIOA时钟使能,选择SPI1，对应PA4，PA5，PA6，PA7 
  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );// SPI1时钟使能 
	
	
  // 初始化GPIOA，PA5/6/7都设置复用推挽输出AF_PP
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;           // 复用推挽输出AF_PP
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);  // PA5/6/7置高电平
	
	
  // 初始化SPI函数
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// 设置SPI单向或双向的数据模式:SPI设置为双线双向全双工
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;		// SPI1设为主机
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;// 针对SPI_CR1寄存器的DFF位，设置数据帧大小为8位
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;		  // 针对SPI_CR1寄存器的CPOL位，串行同步时钟的空闲状态为高电平
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;		  // 针对SPI_CR1寄存器的CPHA位，串行同步时钟的第二个跳变沿（即上升沿）数据被采样
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;			  // 针对SPI_CR1寄存器的SSM位，NSS信号由软件（使用SSI位）管理
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;// 针对SPI_CR1寄存器的BR位，波特率预分频值为256，最低速率	
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// 针对SPI_CR1寄存器的LSBFIRST位，数据传输从MSB位开始
  SPI_InitStruct.SPI_CRCPolynomial = 7;		      // 针对SPI_CRCPR寄存器的CRCPOLY位，设为0x0007，为复位值
  SPI_Init(SPI1, &SPI_InitStruct);  
  SPI_Cmd(SPI1, ENABLE);                        // 使能SPI外设  
	
} 


/**************************************************
函数名称：u8 SPI1_ReadWriteByte(u8 TxData)
函数功能：SPI1读写一个字节函数
入口参数：TxData:要写入的字节
返回参数：读取到的字节
***************************************************/
u8 SPI1_ReadWriteByte(u8 TxData)
{		
  u8 retry=0;	
  // 检查SPI_SR寄存器的TXE位（发送缓冲为空），其值0时为非空，1时为空	
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) 
  {
	  retry++;                           // 发送缓冲为空时，retry++
	  if(retry>200)return 0;
  }			  
  SPI_I2S_SendData(SPI1, TxData);       // 通过外设SPI2发送一个数据
  retry=0;
  // 检查SPI_SR寄存器的RXNE位（接收缓冲为空），其值0时为空，1时为非空
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) 
  {
	   retry++;                         // 当接收缓冲为非空时，retry++
	   if(retry>200)return 0;
  }	  						    
  return SPI_I2S_ReceiveData(SPI1);     // 返回通过SPIx最近接收的数据					    
}


/**************************************************
函数名称：SPI2_Init()
函数功能：SPI2初始化函数：配置成从机模式
入口参数：无
返回参数：无
***************************************************/
void SPI2_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;  
  SPI_InitTypeDef  SPI_InitStruct;   
  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );     // GPIOB时钟使能,选择SPI2，对应PB12，PB13，PB14，PB15 
  RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );     // SPI2时钟使能 
	
  // 初始化GPIOB，PB13/14/15都设置复用推挽输出AF_PP，PB14对应MISO，最好设为带上拉输入
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;                  // 复用推挽输出AF_PP
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);      // PB13/14/15置高电平
	
  // 初始化SPI函数
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// 设置SPI单向或双向的数据模式:SPI设置为双线双向全双工
  SPI_InitStruct.SPI_Mode = SPI_Mode_Slave;		  // SPI2设为从机
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;// 针对SPI_CR1寄存器的DFF位，设置数据帧大小为8位
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;		  // 针对SPI_CR1寄存器的CPOL位，串行同步时钟的空闲状态为高电平
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;		  // 针对SPI_CR1寄存器的CPHA位，串行同步时钟的第二个跳变沿（即上升沿）数据被采样
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;			  // 针对SPI_CR1寄存器的SSM位，NSS信号由软件（使用SSI位）管理
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;// 针对SPI_CR1寄存器的BR位，波特率预分频值为256，最低速率
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// 针对SPI_CR1寄存器的LSBFIRST位，数据传输从MSB位开始
  SPI_InitStruct.SPI_CRCPolynomial = 7;		      // 针对SPI_CRCPR寄存器的CRCPOLY位，设为0x0007，为复位值
  SPI_Init(SPI2, &SPI_InitStruct);  
  SPI_Cmd(SPI2, ENABLE);                        // 使能SPI外设   
	
} 


/**************************************************
函数名称：u8 SPI2_ReadWriteByte(u8 TxData)
函数功能：SPI2读写一个字节函数
入口参数：TxData:要写入的字节
返回参数：读取到的字节
***************************************************/
u8 SPI2_ReadWriteByte(u8 TxData)
{		
  u8 retry=0;	
  // 检查SPI_SR寄存器的TXE位（发送缓冲为空），其值0时为非空，1时为空	
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 
  {
		retry++;                           // 发送缓冲为空时，retry++
		if(retry>200)return 0;
  }			  
  SPI_I2S_SendData(SPI2, TxData);       // 通过外设SPI2发送一个数据
  retry=0;
  // 检查SPI_SR寄存器的RXNE位（接收缓冲为空），其值0时为空，1时为非空
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) 
  {
		retry++;                           // 当接收缓冲为非空时，retry++
		if(retry>200)return 0;
  }	  						    
  return SPI_I2S_ReceiveData(SPI2);     // 返回通过SPIx最近接收的数据					    
}


/***********************************************************************
函数名称：SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
函数功能：SPI1速度设置函数  SPI速度=fAPB2/分频系数 fAPB2时钟一般为84Mhz
入口参数：SPI_BaudRatePrescaler  
					SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
返回参数：无
************************************************************************/
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
} 


/***********************************************************************
函数名称：SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
函数功能：SPI2速度设置函数  SPI速度=fAPB2/分频系数 fAPB2时钟一般为84Mhz
入口参数：SPI_BaudRatePrescaler  
					SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
返回参数：无
************************************************************************/
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	SPI2->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SPI2,ENABLE); //使能SPI1
} 


/**************************************************
函数名称：SPI_text(void)
函数功能：SPI测试函数
入口参数：无
返回参数：无
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



