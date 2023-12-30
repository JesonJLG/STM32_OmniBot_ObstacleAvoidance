#include "serial.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "beep.h"
#include "usart.h"

u8 Pwm_RxPacket[20];	//"P[...]\r\n"	Pwm_RxPacket[0]Ϊpwm_limit	APP�ٶȻ���
u8 Lr_RxFlag, Rr_RxFlag, Pwm_RxFlag, Pr_RxFlag;	//����ҡ�ˣ������ǵ����ݽ��ձ�־λ

//ע��:ʹ������ģ��Ĵ��ڲ�����ʹ��9600,���ܳ���9600������

u8 flag = 0;		//��������������
u8 mode_flag = 0;	//С������ģʽ��־λ

char Lr_Buf[10] = { 0 };	//��ҡ�����ݽ��ջ�����
char Rr_Buf[10] = { 0 };	//��ҡ�����ݽ��ջ�����
char Pitch_Roll_Buf[20] = { 0 };	//APPƫ�������ݽ��ջ�����

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
����Ҫ���� ������֡���� ��ң�ض˷�����ң������ �������浽����ȫ�ֹ����������Lr_Buf[10]��Rr_Buf[10]��Pitch_Roll_Buf[20]�У�
��ڲ�������
���ز�������
***************************************************/
void USART2_IRQHandler(void)
{
    u8 RxData;
    static u8 t = 0, n1 = 0, n2 = 0;	//�ƴ� ��Ϊ���˱�־λʹ��
    static u8 i = 0, j = 0, k = 0;		//�����������±꣩
    static char temp_buf1[10] = { 0 }, temp_buf2[10] = { 0 }, temp_buf3[20] = { 0 };	//������֡

//    static u8 n4 = 0, a = 0, pwm_flag = 0;
//    static char temp_buf4[10] = { 0 };
//	static u8 n5 = 0, b = 0, b_flag = 0;
//	static char temp_buf5[10] = { 0 };
//	static u8 n6 = 0, c = 0, c_flag = 0;
//	static char temp_buf6[10] = { 0 };

    /*------------try-------------*/
    static uint8_t RxState = 0;
    static uint8_t pRxPacket = 0;

    if (USART_GetITStatus(USARTX, USART_IT_RXNE) != RESET)
    {
        /*------------��־λ�ĸ�ֵ-------------*/
        RxData = USART_ReceiveData(USARTX);
        //USART_SendData(USARTX, RxData);
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

        /*------------����ң��ģʽ�µ�ң������-------------*/
        if (mode_flag != 2)				//��ģʽ��Ϊ������Ӧģʽʱ(��ң��ģʽ)
        {
            if (RxData == 'L')			//���յ�֡ͷΪL��һ֡����
            {
                Lr_RxFlag = 1;			//��ҡ�˱�־λ ��1
            }
            if (Lr_RxFlag == 1)			//��ʼ������һ֡����
            {
                temp_buf1[i] = RxData;	//��ǰ�����ַ� ���浽����ĵ�i��Ԫ�� ����жϽ���һ����֡����
                i++;
                if (RxData == '*')		//֡βΪ*ʱһ֡���ݽ������
                {
                    if (n1 == 0)		//���ˣ�ȫ�֣�����һ��ҡ�����ݣ�������ջ�������
                    {
                        memset(Lr_Buf, 0, 10);		//���Lr_Buf������
                        memset(temp_buf1, 0, 10);	//���temp_buf1������
                        n1 = 1;						//ͬʱ�ƴ������(���治��0?)
                    }
                    strcpy(Lr_Buf, temp_buf1);		//�ѽ��յ���һ֡���ݼ���õ�֡ ���帳��Rr_Buf����
                    Lr_RxFlag = 0;					//��ҡ�����ݽ�����ϣ���ձ�־λ
                    i = 0;							//ͬʱ������������
                }
            }

            if (RxData == 'R')			//���յ�֡ͷΪR��һ֡����
            {
                Rr_RxFlag = 1;
            }
            if (Rr_RxFlag == 1)			//��ʼ������һ֡����
            {
                temp_buf2[j] = RxData;	//��ǰ�����ַ� ���浽����ĵ�i��Ԫ�� ����жϽ���һ����֡����
                j++;
                if (RxData == '*')		//֡βΪ*ʱһ֡���ݽ������
                {
                    if (n2 == 0)		//���ˣ�ȫ�֣�����һ��ҡ�����ݣ�������ջ�������
                    {
                        memset(Rr_Buf, 0, 10);		//���Rr_Buf������
                        memset(temp_buf2, 0, 10);	//���temp_buf2������
                        n2 = 1;						//ͬʱ�ƴ������(���治��0?)
                    }
                    strcpy(Rr_Buf, temp_buf2);		//�ѽ��յ���һ֡���ݼ���õ�֡ ���帳��Rr_Buf����
                    Rr_RxFlag = 0;					//��ҡ�����ݽ�����ϣ���ձ�־λ
                    j = 0;							//ͬʱ������������
                }
            }

            /*------------����pwm_limit����-------------*/
            /*------------try-------------*/
            if (RxState == 0)
            {
                if (RxData == 'P' && Pwm_RxFlag == 0)
                {
					BEEP_Tick();
                    RxState = 1;
                    pRxPacket = 0;
                }
            }
            else if (RxState == 1)
            {
                if (RxData == '*')
                {
                    RxState = 0;
                    Pwm_RxPacket[pRxPacket] = '\0';
                    Pwm_RxFlag = 1;
                }
                else
                {
                    Pwm_RxPacket[pRxPacket] = RxData;
                    pRxPacket ++;
                }
            }
//            else if (RxState == 2)
//            {
//                if (RxData == '\n')
//                {
//                    RxState = 0;
//                    Pwm_RxPacket[pRxPacket] = '\0';
//                    Pwm_RxFlag = 1;
//                }
//            }

//            if(RxData == 'P')
//            {
//                pwm_flag = 1;
//            }
//            if(pwm_flag == 1)
//            {
//                temp_buf4[j] = RxData;	//��ǰ�����ַ� ���浽����ĵ�i��Ԫ�� ����жϽ���һ����֡����
//                a++;
//                if (RxData == '*')		//֡βΪ*ʱһ֡���ݽ������
//                {
//                    //memset(Rr_Buf, 0, 10);		//���Rr_Buf������
//                    memset(temp_buf4, 0, 10);	//���temp_buf2������

//                    strcpy(Rr_Buf, temp_buf4);		//�ѽ��յ���һ֡���ݼ���õ�֡ ���帳��Rr_Buf����
//                    pwm_flag = 0;					//��ҡ�����ݽ�����ϣ���ձ�־λ
//                    j = 0;							//ͬʱ������������
//                }
//            }
        }
        /*------------������Ӧģʽ-------------*/
        else	//mode_flag == 2	�� ��ǰģʽΪ������Ӧģʽ
        {
            if (RxData == 'A')	//���յ�֡ͷΪA��һ֡����(������Ӧģʽ����)
            {
                Pr_RxFlag = 1;
                memset(Pitch_Roll_Buf, 0, 20);
                memset(temp_buf3, 0, 20);
            }
            if (Pr_RxFlag == 1)
            {
                temp_buf3[k] = RxData;
                k++;
                if (RxData == '*')
                {
                    strcpy(Pitch_Roll_Buf, temp_buf3);
                    Pr_RxFlag = 0;
                    k = 0;
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
