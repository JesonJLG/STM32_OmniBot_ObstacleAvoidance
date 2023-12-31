#include "serial.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "beep.h"
#include "usart.h"



//ע��:ʹ������ģ��Ĵ��ڲ�����ʹ��9600,���ܳ���9600������

u8 flag = 0;		//��������������
u8 mode_flag = 0;	//С������ģʽ��־λ

char Lr_RxPacket[10] = { 0 };		//��ҡ�����ݽ��ջ�����
char Rr_RxPacket[10] = { 0 };		//��ҡ�����ݽ��ջ�����
char Pitch_Roll_RxPacket[20] = { 0 };	//APPƫ�������ݽ��ջ�����
char Pwm_RxPacket[10];				//"P[...]\r\n"	Pwm_RxPacket[0]Ϊpwm_limit	APP�ٶȻ���
char Lr_RxFlag, Rr_RxFlag, Pwm_RxFlag, Pr_RxFlag;	//����ҡ�ˣ������ǵ����ݽ��ձ�־λ


void Serial_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = USART_TX;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
    GPIO_Init(USART_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = USART_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
    GPIO_Init(USART_PORT, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;						//���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;		 	//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;        		//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USARTX, &USART_InitStructure);     //��ʼ������2
//    USART_ITConfig(USARTX, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USARTX, ENABLE);                    //ʹ�ܴ���2
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
�������ƣ�USART2_IRQHandler(void)
�������ܣ�����2�жϺ���	���������������ݵĽ���
����Ҫ���� ������֡���� ��ң�ض˷�����ң������ �������浽����ȫ�ֹ����������Lr_RxPacket[10]��Rr_RxPacket[10]��Pitch_Roll_RxPacket[20]�У�
��ڲ�������
���ز�������
***************************************************/
void USART2_IRQHandler(void)
{
    u8 RxData;
    static u8 t = 0, n1 = 0, n2 = 0, n3 = 0;	//�ƴ� ��Ϊ���˱�־λʹ��
    static char Lr_RxBuf[10], Rr_RxBuf[10], Pitch_Roll_RxBuf[20],Pwm_RxBuf[10];	//������֡
    static char Lr_index, Rr_index, Pr_index, Pwm_index;	//�����������±꣩	������֡

    if (USART_GetITStatus(USARTX, USART_IT_RXNE) != RESET)
    {
        /*------------��־λ�ĸ�ֵ-------------*/
        RxData = USART_ReceiveData(USARTX);
        //USART_SendData(USART1, RxData);
        if (RxData == 'a')
        {
            mode_flag = 1;
            BEEP_Tick();
        }
        else if (RxData == 'b')
        {
            mode_flag = 2;	//������Ӧģʽ(�ֻ�ң��APP����ʵ��)
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

        /*------------��־λ�Ĵ���-------------*/
        if (t == 0 && mode_flag == 4)	//���ˣ�ȫ�֣�����һ��mode_flag=4
        {
            mode_flag = 0;				//����ͨ������mode_flag��־λ��ʵ��һ�������ˡ���Ч��
            t = 1;						//ͬʱ�ƴ������(���治��0?)
        }

        /*------------����ң��ģʽ�µ�ң�����ݣ���������-------------*/
        if (mode_flag != 2)				//��ģʽ��Ϊ������Ӧģʽʱ(��ң��ģʽ)
        {
//			/*------------������ҡ������-------------*/
//            if (RxData == 'L')			//���յ�֡ͷΪL��һ֡����
//            {
//                Lr_RxFlag = 1;			//��ҡ�˱�־λ ��1
//            }
//            if (Lr_RxFlag == 1)			//��ʼ������һ֡����
//            {
//                Lr_RxBuf[Lr_index] = RxData;	//��ǰ�����ַ� ���浽����ĵ�Lr_index��Ԫ�� ����жϽ���һ����֡����
//                Lr_index++;
//                if (RxData == '*')		//֡βΪ*ʱһ֡���ݽ������
//                {
//                    if (n1 == 0)		//���ˣ�ȫ�֣�����һ��ҡ�����ݣ�������ջ�������
//                    {
//                        memset(Lr_RxPacket, 0, 10);		//���Lr_RxPacket������
//                        memset(Lr_RxBuf, 0, 10);	//���Lr_RxBuf������
//                        n1 = 1;						//ͬʱ�ƴ������(���治��0?)
//                    }
//                    strcpy(Lr_RxPacket, Lr_RxBuf);		//�ѽ��յ���һ֡���ݼ���õ�֡ ���帳��Rr_RxPacket����
//                    Lr_RxFlag = 0;					//��ҡ�����ݽ�����ϣ���ձ�־λ
//                    Lr_index = 0;							//ͬʱ������������
//                }
//            }
//			/*------------������ҡ������-------------*/
//            if (RxData == 'R')			//���յ�֡ͷΪR��һ֡����
//            {
//                Rr_RxFlag = 1;
//            }
//            if (Rr_RxFlag == 1)			//��ʼ������һ֡����
//            {
//                Rr_RxBuf[Rr_index] = RxData;	//��ǰ�����ַ� ���浽����ĵ�i��Ԫ�� ����жϽ���һ����֡����
//                Rr_index++;
//                if (RxData == '*')		//֡βΪ*ʱһ֡���ݽ������
//                {
//                    if (n2 == 0)		//���ˣ�ȫ�֣�����һ��ҡ�����ݣ�������ջ�������
//                    {
//                        memset(Rr_RxPacket, 0, 10);		//���Rr_RxPacket������
//                        memset(Rr_RxBuf, 0, 10);		//���Rr_RxBuf������
//                        n2 = 1;							//ͬʱ�ƴ������(���治��0?)
//                    }
//                    strcpy(Rr_RxPacket, Rr_RxBuf);		//�ѽ��յ���һ֡���ݼ���õ�֡ ���帳��Rr_RxPacket����
//                    Rr_RxFlag = 0;						//��ҡ�����ݽ�����ϣ���ձ�־λ
//                    Rr_index = 0;						//ͬʱ������������
//                }
//            }
			/*------------����pwm_limit����-------------*/
//			if (RxData == 'P')			//���յ�֡ͷΪP��һ֡����
//            {
//                Pwm_RxFlag = 1;
//				  //BEEP_Tick();	//���������ﲻ������ʱ������Ӱ�촮�����ݽ���
//            }
//            if (Pwm_RxFlag == 1)			//��ʼ������һ֡����
//            {
//                Pwm_RxBuf[Pwm_index] = RxData;	//��ǰ�����ַ� ���浽����ĵ�i��Ԫ�� ����жϽ���һ����֡����
//                Pwm_index++;
//                if (RxData == '*')		//֡βΪ*ʱһ֡���ݽ������
//                {
////                    if (n3 == 0)		//���ˣ�ȫ�֣�����һ��ҡ�����ݣ�������ջ�������
////                    {
////                        memset(Pwm_RxPacket, 0, 10);		//���Pwm_RxPacket������
////                        memset(Pwm_RxBuf, 0, 10);		//���Pwm_RxBuf������
////                        n3 = 1;							//ͬʱ�ƴ������(���治��0?)
////                    }
//                    strcpy(Pwm_RxPacket, Pwm_RxBuf);		//�ѽ��յ���һ֡���ݼ���õ�֡ ���帳��Pwm_RxPacket����
//                    Pwm_RxFlag = 0;						//��ҡ�����ݽ�����ϣ���ձ�־λ
//                    Pwm_index = 0;						//ͬʱ������������
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
        /*------------������Ӧģʽ-------------*/
        else	//mode_flag == 2	�� ��ǰģʽΪ������Ӧģʽ
        {
            if (RxData == 'A')	//���յ�֡ͷΪA��һ֡����(������Ӧģʽ����)
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

/*------------�ȴ��������ӳɹ��������ж�-------------*/
void Wait_BT_CONNECTED(void)
{
    char temp;
    char sta = 0;
    while(1)
    {
        while (USART_GetFlagStatus(USARTX, USART_FLAG_RXNE) != RESET) // �ж��Ƿ������ݿɶ�
        {
            temp = USART_ReceiveData(USARTX); // ��ȡһ���ֽ�
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
                    USART_ITConfig(USARTX, USART_IT_RXNE, ENABLE);//�������ڽ����ж�;
                    BEEP_Tick();
                    return ;
                }
                else			sta = 0;
                break;
            default:
                sta = 0;
                printf("δ����CONNECTED\r\n");
                break;
            }
        }
    }

}
