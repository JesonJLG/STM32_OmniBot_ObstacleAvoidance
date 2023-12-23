#include "sys.h"
#include "usart.h"	
#include "string.h"

//注意:使用蓝牙模块的串口波特率使用9600,不能超过9600波特率

u8 flag = 0;		//于蓝牙串口无用
u8 mode_flag = 0;	//小车控制模式标志位

char Lx_Buf[10] = { 0 };	//左摇杆数据接收缓冲区
char Rx_Buf[10] = { 0 };	//右摇杆数据接收缓冲区
char Pitch_Roll_Buf[20];	//APP偏航角数据接收缓冲区


void USART_SendByte(USART_TypeDef* USARTx, uint16_t Data)//发送一个字节
{
	USARTx->DR = (Data & (uint16_t)0x01FF);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

/**************************************************
函数名称：USART1_Init(u32 bound)
函数功能：串口1初始化
入口参数：bound  波特率
返回参数：无
***************************************************/
void USART1_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 						//PA9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50ZMHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;						//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//初始化NVIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;								 //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		 //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;        //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure);     //初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

/**************************************************
函数名称：USART2_Init(u32 bound)
函数功能：串口2初始化
入口参数：bound  波特率
返回参数：无
***************************************************/
void USART2_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART2时钟

	//USART2_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 						//PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50ZMHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化GPIOA.2

	//USART2_RX	  GPIOA.3初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;						  //PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化GPIOA.3  

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//初始化NVIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;								 //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		 //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;        //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART2, &USART_InitStructure);     //初始化串口2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART2, ENABLE);                    //使能串口2 
}

/**************************************************
函数名称：USART3_Init(u32 bound)
函数功能：串口3初始化
入口参数：bound  波特率
返回参数：无
***************************************************/
void USART3_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能USART3时钟

	//USART3_TX   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 						//PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50ZMHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);								//初始化GPIOB.10

	//USART3_RX	  GPIOB.11初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;						  //PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);								//初始化GPIOB.11  

	//Usart3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//初始化NVIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;								 //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		 //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;        //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART3, &USART_InitStructure);     //初始化串口3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART3, ENABLE);                    //使能串口3 

}

/**************************************************
函数名称：USART1_IRQHler(void) 
函数功能：串口1中断函数	进行蓝牙串口数据的解析
（主要就是 进行组帧操作 将遥控端发来的遥控数据 解析保存到三个全局共享数组变量Lx_Buf[10]、Rx_Buf[10]、Pitch_Roll_Buf[20]中）
入口参数：无
返回参数：无
***************************************************/
void USART1_IRQHandler(void)
{
	u8 temp;
	static u8 t = 0, n1 = 0, n2 = 0;	//计次 作为过滤标志位使用
	static u8 i = 0, j = 0, k = 0;		//数组索引（下标）
	static char temp_buf1[10] = { 0 }, temp_buf2[10] = { 0 }, temp_buf3[20] = { 0 };	//用于组帧
	static u8 Lx_flag = 0, Rx_flag = 0, Pr_flag = 0;	//左右摇杆，陀螺仪接收数据标志位


	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		/*------------标志位的赋值-------------*/
		temp = USART_ReceiveData(USART1);
		if (temp == 'a')		mode_flag = 1;
		else if (temp == 'b')	mode_flag = 2;	//重力感应模式(手机遥控APP可以实现)
		else if (temp == 'c')	mode_flag = 3;
		else if (temp == 'd')	mode_flag = 4;
		else if (temp == 'e')	mode_flag = 5;
		else if (temp == 'f')	mode_flag = 6;
		else if (temp == 'g')	mode_flag = 7;

		/*------------标志位的处理-------------*/
		if (t == 0 && mode_flag == 4)	//过滤（全局？）第一次mode_flag=4
		{
			mode_flag = 0;				//这里通过重置mode_flag标志位，实现一个“过滤”的效果
			t = 1;						//同时计次做标记(后面不清0?)
		}

		/*------------处理遥控模式下的遥杆数据-------------*/
		if (mode_flag != 2)				//当模式不为重力感应模式时(即遥控模式)
		{
			if (temp == 'L')			//接收到帧头为L的一帧数据
			{
				Lx_flag = 1;			//左摇杆标志位 置1
			}
			if (Lx_flag == 1)			//开始接收这一帧数据
			{
				temp_buf1[i] = temp;	//当前接受字符 保存到数组的第i个元素 多次中断进行一个组帧操作
				i++;
				if (temp == '*')		//帧尾为*时一帧数据接收完毕
				{
					if (n1 == 0)		//过滤（全局？）第一次摇杆数据（用来清空缓冲区）
					{
						memset(Lx_Buf, 0, 10);		//清空Lx_Buf缓冲区
						memset(temp_buf1, 0, 10);	//清空temp_buf1缓冲区
						n1 = 1;						//同时计次做标记(后面不清0?)
					}
					strcpy(Lx_Buf, temp_buf1);		//把接收到的一帧数据即组好的帧 整体赋给Rx_Buf数组
					Lx_flag = 0;					//左摇杆数据接收完毕，清空标志位
					i = 0;							//同时重置数组索引
				}
			}

			if (temp == 'R')			//接收到帧头为R的一帧数据
			{
				Rx_flag = 1;
			}
			if (Rx_flag == 1)			//开始接收这一帧数据
			{
				temp_buf2[j] = temp;	//当前接受字符 保存到数组的第i个元素 多次中断进行一个组帧操作
				j++;
				if (temp == '*')		//帧尾为*时一帧数据接收完毕
				{
					if (n2 == 0)		//过滤（全局？）第一次摇杆数据（用来清空缓冲区）
					{
						memset(Rx_Buf, 0, 10);		//清空Rx_Buf缓冲区
						memset(temp_buf2, 0, 10);	//清空temp_buf2缓冲区
						n2 = 1;						//同时计次做标记(后面不清0?)
					}
					strcpy(Rx_Buf, temp_buf2);		//把接收到的一帧数据即组好的帧 整体赋给Rx_Buf数组
					Rx_flag = 0;					//右摇杆数据接收完毕，清空标志位
					j = 0;							//同时重置数组索引
				}
			}
		}
		else	//mode_flag == 2	即 当前模式为重力感应模式
		{
			if (temp == 'A')	//接收到帧头为A的一帧数据(重力感应模式数据)
			{
				Pr_flag = 1;
				memset(Pitch_Roll_Buf, 0, 20);
				memset(temp_buf3, 0, 20);
			}
			if (Pr_flag == 1)
			{
				temp_buf3[k] = temp;
				k++;
				if (temp == '*')
				{
					strcpy(Pitch_Roll_Buf, temp_buf3);
					Pr_flag = 0;
					k = 0;
				}
			}
		}
	}
}

/**************************************************
函数名称：USART2_IRQHler(void) 
函数功能：串口2中断函数
入口参数：无
返回参数：无
***************************************************/
void USART2_IRQHandler(void)
{
	u8 temp;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		temp = USART_ReceiveData(USART2);
		switch (temp)
		{
		case 'a':flag = 1;break;
		case 'b':flag = 2;break;
		}
	}
	USART_SendByte(USART2, temp);
}


/**************************************************
函数名称：USART3_IRQHler(void) 
函数功能：串口3中断函数
入口参数：无
返回参数：无
***************************************************/
void USART3_IRQHandler(void)
{
	u8 temp;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		temp = USART_ReceiveData(USART3);
		switch (temp)
		{
		case 'a':flag = 1;break;
		case 'b':flag = 2;break;
		}
	}
	USART_SendByte(USART3, temp);
}


/*************************串口发送函数*************************************/

void USART1_Send_Byte(u8 Data) {
	USART_SendData(USART1, Data);
	return;
}

void USART1_Send_nByte(u8* Data, u16 size) {
	u16 i = 0;
	for (i = 0; i < size; i++) {
		USART_SendData(USART1, Data[i]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
	return;
}

void USART1_Send_Str(u8* Data) {
	while (*Data) {
		USART_SendData(USART1, *Data++);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
	return;
}

void USART2_Send_Byte(u8 Data) {
	USART_SendData(USART2, Data);
	return;
}

void USART2_Send_nByte(u8* Data, u16 size) {
	u16 i = 0;
	for (i = 0; i < size; i++) {
		USART_SendData(USART2, Data[i]);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
	return;
}

void USART2_Send_Str(u8* Data) {
	while (*Data) {
		USART_SendData(USART2, *Data++);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
	return;
}

void USART3_Send_Byte(u8 Data) {
	USART_SendData(USART3, Data);
	return;
}

void USART3_Send_nByte(u8* Data, u16 size) {
	u16 i = 0;
	for (i = 0; i < size; i++) {
		USART_SendData(USART3, Data[i]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	}
	return;
}

void USART3_Send_Str(u8* Data) {
	while (*Data) {
		USART_SendData(USART3, *Data++);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	}
	return;
}



/**************************************************
函数名称：fputc(int ch,FILE *f)
函数功能：串口重定向
入口参数：无
返回参数：无
***************************************************/
#pragma import(__use_no_semihosting)

struct __FILE {
	int handle;
};

FILE __stdout;
_sys_exit(int x)
{
	x = x;
}

int fputc(int ch, FILE* f)
{
	USART_SendData(USART1, (uint8_t)ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return (ch);
}

int fgetc(FILE* f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return ((int)USART_ReceiveData(USART1));
}


