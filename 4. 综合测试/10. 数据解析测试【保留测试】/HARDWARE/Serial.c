#include "serial.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "beep.h"
#include "usart.h"



//注意:使用蓝牙模块的串口波特率使用9600,不能超过9600波特率

u8 flag = 0;		//于蓝牙串口无用
u8 mode_flag = 0;	//小车控制模式标志位

char Lr_RxPacket[10] = { 0 };		//左摇杆数据接收缓冲区
char Rr_RxPacket[10] = { 0 };		//右摇杆数据接收缓冲区
char Pitch_Roll_RxPacket[20] = { 0 };	//APP偏航角数据接收缓冲区
char Pwm_RxPacket[10];				//"P[...]\r\n"	Pwm_RxPacket[0]为pwm_limit	APP速度滑条
char Lr_RxFlag, Rr_RxFlag, Pwm_RxFlag, Pr_RxFlag;	//左右摇杆，陀螺仪等数据接收标志位


void Serial_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = USART_TX;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
    GPIO_Init(USART_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = USART_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
    GPIO_Init(USART_PORT, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;						//串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;		 	//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;        		//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USARTX, &USART_InitStructure);     //初始化串口2
//    USART_ITConfig(USARTX, USART_IT_RXNE, ENABLE);//开启串口接受中断
    USART_Cmd(USARTX, ENABLE);                    //使能串口2
}

void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(USARTX, Byte);
    while (USART_GetFlagStatus(USARTX, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i ++)
    {
        Serial_SendByte(Array[i]);
    }
}

void Serial_SendString(char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i ++)
    {
        Serial_SendByte(String[i]);
    }
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y --)
    {
        Result *= X;
    }
    return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i ++)
    {
        Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
    }
}

//int fputc(int ch, FILE *f)
//{
//	Serial_SendByte(ch);
//	return ch;
//}

void Serial_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    Serial_SendString(String);
}

/**************************************************
函数名称：USART2_IRQHandler(void)
函数功能：串口2中断函数	进行蓝牙串口数据的解析
（主要就是 进行组帧操作 将遥控端发来的遥控数据 解析保存到三个全局共享数组变量Lr_RxPacket[10]、Rr_RxPacket[10]、Pitch_Roll_RxPacket[20]中）
入口参数：无
返回参数：无
***************************************************/
void USART2_IRQHandler(void)
{
    u8 RxData;
    static u8 t = 0, n1 = 0, n2 = 0, n3 = 0;	//计次 作为过滤标志位使用
    static char Lr_RxBuf[10], Rr_RxBuf[10], Pitch_Roll_RxBuf[20],Pwm_RxBuf[10];	//用于组帧
    static char Lr_index, Rr_index, Pr_index, Pwm_index;	//数组索引（下标）	用于组帧

    if (USART_GetITStatus(USARTX, USART_IT_RXNE) != RESET)
    {
        /*------------标志位的赋值-------------*/
        RxData = USART_ReceiveData(USARTX);
        //USART_SendData(USART1, RxData);
        if (RxData == 'a')
        {
            mode_flag = 1;
            BEEP_Tick();
        }
        else if (RxData == 'b')
        {
            mode_flag = 2;	//重力感应模式(手机遥控APP可以实现)
            BEEP_Tick();
        }
        else if (RxData == 'c')
        {
            mode_flag = 3;
            BEEP_Tick();
        }
        else if (RxData == 'd')
        {
            mode_flag = 4;
            BEEP_Tick();
        }
        else if (RxData == 'e')
        {
            mode_flag = 5;
            BEEP_Tick();
        }
        else if (RxData == 'f')
        {
            mode_flag = 6;
            BEEP_Tick();
        }
        else if (RxData == 'g')
        {
            mode_flag = 7;
            BEEP_Tick();
        }

        /*------------标志位的处理-------------*/
        if (t == 0 && mode_flag == 4)	//过滤（全局？）第一次mode_flag=4
        {
            mode_flag = 0;				//这里通过重置mode_flag标志位，实现一个“过滤”的效果
            t = 1;						//同时计次做标记(后面不清0?)
        }

        /*------------处理遥控模式下的遥杆数据（分类打包）-------------*/
        if (mode_flag != 2)				//当模式不为重力感应模式时(即遥控模式)
        {
//			/*------------解析左摇杆数据-------------*/
//            if (RxData == 'L')			//接收到帧头为L的一帧数据
//            {
//                Lr_RxFlag = 1;			//左摇杆标志位 置1
//            }
//            if (Lr_RxFlag == 1)			//开始接收这一帧数据
//            {
//                Lr_RxBuf[Lr_index] = RxData;	//当前接受字符 保存到数组的第Lr_index个元素 多次中断进行一个组帧操作
//                Lr_index++;
//                if (RxData == '*')		//帧尾为*时一帧数据接收完毕
//                {
//                    if (n1 == 0)		//过滤（全局？）第一次摇杆数据（用来清空缓冲区）
//                    {
//                        memset(Lr_RxPacket, 0, 10);		//清空Lr_RxPacket缓冲区
//                        memset(Lr_RxBuf, 0, 10);	//清空Lr_RxBuf缓冲区
//                        n1 = 1;						//同时计次做标记(后面不清0?)
//                    }
//                    strcpy(Lr_RxPacket, Lr_RxBuf);		//把接收到的一帧数据即组好的帧 整体赋给Rr_RxPacket数组
//                    Lr_RxFlag = 0;					//左摇杆数据接收完毕，清空标志位
//                    Lr_index = 0;							//同时重置数组索引
//                }
//            }
//			/*------------解析右摇杆数据-------------*/
//            if (RxData == 'R')			//接收到帧头为R的一帧数据
//            {
//                Rr_RxFlag = 1;
//            }
//            if (Rr_RxFlag == 1)			//开始接收这一帧数据
//            {
//                Rr_RxBuf[Rr_index] = RxData;	//当前接受字符 保存到数组的第i个元素 多次中断进行一个组帧操作
//                Rr_index++;
//                if (RxData == '*')		//帧尾为*时一帧数据接收完毕
//                {
//                    if (n2 == 0)		//过滤（全局？）第一次摇杆数据（用来清空缓冲区）
//                    {
//                        memset(Rr_RxPacket, 0, 10);		//清空Rr_RxPacket缓冲区
//                        memset(Rr_RxBuf, 0, 10);		//清空Rr_RxBuf缓冲区
//                        n2 = 1;							//同时计次做标记(后面不清0?)
//                    }
//                    strcpy(Rr_RxPacket, Rr_RxBuf);		//把接收到的一帧数据即组好的帧 整体赋给Rr_RxPacket数组
//                    Rr_RxFlag = 0;						//右摇杆数据接收完毕，清空标志位
//                    Rr_index = 0;						//同时重置数组索引
//                }
//            }
			/*------------解析pwm_limit变量-------------*/
//			if (RxData == 'P')			//接收到帧头为P的一帧数据
//            {
//                Pwm_RxFlag = 1;
//				  //BEEP_Tick();	//！！！这里不能有延时，否则影响串口数据接收
//            }
//            if (Pwm_RxFlag == 1)			//开始接收这一帧数据
//            {
//                Pwm_RxBuf[Pwm_index] = RxData;	//当前接受字符 保存到数组的第i个元素 多次中断进行一个组帧操作
//                Pwm_index++;
//                if (RxData == '*')		//帧尾为*时一帧数据接收完毕
//                {
////                    if (n3 == 0)		//过滤（全局？）第一次摇杆数据（用来清空缓冲区）
////                    {
////                        memset(Pwm_RxPacket, 0, 10);		//清空Pwm_RxPacket缓冲区
////                        memset(Pwm_RxBuf, 0, 10);		//清空Pwm_RxBuf缓冲区
////                        n3 = 1;							//同时计次做标记(后面不清0?)
////                    }
//                    strcpy(Pwm_RxPacket, Pwm_RxBuf);		//把接收到的一帧数据即组好的帧 整体赋给Pwm_RxPacket数组
//                    Pwm_RxFlag = 0;						//右摇杆数据接收完毕，清空标志位
//                    Pwm_index = 0;						//同时重置数组索引
//                }
//            }

            /*------------try-------------*/
			static uint8_t RxState = 0;
			static uint8_t pRxPacket = 0;
			
            if (RxState == 0)
            {
                if (RxData == '@' && Pwm_RxFlag == 0)
                {
					//BEEP_Tick();
                    RxState = 1;
                    pRxPacket = 0;
                }
            }
            else if (RxState == 1)
            {
                if (RxData == '\r')
                {
                    RxState = 2;
                }
                else
                {
                    Pwm_RxPacket[pRxPacket] = RxData;
                    pRxPacket ++;
                }
            }
            else if (RxState == 2)
            {
                if (RxData == '\n')
                {
                    RxState = 0;
                    Pwm_RxPacket[pRxPacket] = '\0';
                    Pwm_RxFlag = 1;		
                }
            }
        }
        /*------------重力感应模式-------------*/
        else	//mode_flag == 2	即 当前模式为重力感应模式
        {
            if (RxData == 'A')	//接收到帧头为A的一帧数据(重力感应模式数据)
            {
                Pr_RxFlag = 1;
                memset(Pitch_Roll_RxPacket, 0, 20);
                memset(Pitch_Roll_RxBuf, 0, 20);
            }
            if (Pr_RxFlag == 1)
            {
                Pitch_Roll_RxBuf[Pr_index] = RxData;
                Pr_index++;
                if (RxData == '*')
                {
                    strcpy(Pitch_Roll_RxPacket, Pitch_Roll_RxBuf);
                    Pr_RxFlag = 0;
                    Pr_index = 0;
                }
            }
        }
    }
}

//void USART1_IRQHandler(void)
//{
//	static uint8_t RxState = 0;
//	static uint8_t pRxPacket = 0;
//	if (USART_GetITStatus(USARTX, USART_IT_RXNE) == SET)
//	{
//		uint8_t RxData = USART_ReceiveData(USARTX);
//
//		if (RxState == 0)
//		{
//			if (RxData == '@' && Pwm_RxFlag == 0)
//			{
//				RxState = 1;
//				pRxPacket = 0;
//			}
//		}
//		else if (RxState == 1)
//		{
//			if (RxData == '\r')
//			{
//				RxState = 2;
//			}
//			else
//			{
//				Pwm_RxPacket[pRxPacket] = RxData;
//				pRxPacket ++;
//			}
//		}
//		else if (RxState == 2)
//		{
//			if (RxData == '\n')
//			{
//				RxState = 0;
//				Pwm_RxPacket[pRxPacket] = '\0';
//				Pwm_RxFlag = 1;
//			}
//		}
//
//		USART_ClearITPendingBit(USARTX, USART_IT_RXNE);
//	}
//}

/*------------等待蓝牙连接成功，开启中断-------------*/
void Wait_BT_CONNECTED(void)
{
    char temp;
    char sta = 0;
    while(1)
    {
        while (USART_GetFlagStatus(USARTX, USART_FLAG_RXNE) != RESET) // 判断是否有数据可读
        {
            temp = USART_ReceiveData(USARTX); // 读取一个字节
            switch (sta)
            {
            case 0:
                if(temp == 'C')	sta = 1;
                else			sta = 0;
                break;
            case 1:
                if(temp == 'O')	sta = 2;
                else			sta = 0;
                break;
            case 2:
                if(temp == 'N')	sta = 3;
                else			sta = 0;
                break;
            case 3:
                if(temp == 'N')	sta = 4;
                else			sta = 0;
                break;
            case 4:
                if(temp == 'E')	sta = 5;
                else			sta = 0;
                break;
            case 5:
                if(temp == 'C')	sta = 6;
                else			sta = 0;
                break;
            case 6:
                if(temp == 'T')	sta = 7;
                else			sta = 0;
                break;
            case 7:
                if(temp == 'E')	sta = 8;
                else			sta = 0;
                break;
            case 8:
                if(temp == 'D')
                {
                    USART_ITConfig(USARTX, USART_IT_RXNE, ENABLE);//开启串口接受中断;
                    BEEP_Tick();
                    return ;
                }
                else			sta = 0;
                break;
            default:
                sta = 0;
                printf("未命中CONNECTED\r\n");
                break;
            }
        }
    }

}
