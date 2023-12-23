#include "usart.h"
#include "stdio.h"
#include "string.h"

//ע��:ʹ������ģ��Ĵ��ڲ�����ʹ��9600,���ܳ���9600������

u8 flag = 0;		//��������������
u8 mode_flag = 0;	//С������ģʽ��־λ

char Lx_Buf[10] = { 0 };	//��ҡ�����ݽ��ջ�����
char Rx_Buf[10] = { 0 };	//��ҡ�����ݽ��ջ�����
char Pitch_Roll_Buf[20] = { 0 };	//APPƫ�������ݽ��ջ�����


void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//    NVIC_Init(&NVIC_InitStructure);	                        //��ʼ��NVIC�Ĵ���

	USART_InitStructure.USART_BaudRate = bound;					//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;       	//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure);     //��ʼ������1
//    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1

}

void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

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

	USART_Init(USART2, &USART_InitStructure);     //��ʼ������2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���2
}

void USART_SendByte(USART_TypeDef* USARTx, uint16_t Data)//����һ���ֽ�
{
	USARTx->DR = (Data & (uint16_t)0x01FF);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

/**************************************************
�������ƣ�USART2_IRQHandler(void) 
�������ܣ�����2�жϺ���	���������������ݵĽ���
����Ҫ���� ������֡���� ��ң�ض˷�����ң������ �������浽����ȫ�ֹ����������Lx_Buf[10]��Rx_Buf[10]��Pitch_Roll_Buf[20]�У�
��ڲ�������
���ز�������
***************************************************/
void USART2_IRQHandler(void)
{
	u8 temp;
	static u8 t = 0, n1 = 0, n2 = 0;	//�ƴ� ��Ϊ���˱�־λʹ��
	static u8 i = 0, j = 0, k = 0;		//�����������±꣩
	static char temp_buf1[10] = { 0 }, temp_buf2[10] = { 0 }, temp_buf3[20] = { 0 };	//������֡
	static u8 Lx_flag = 0, Rx_flag = 0, Pr_flag = 0;	//����ҡ�ˣ������ǽ������ݱ�־λ


	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		/*------------��־λ�ĸ�ֵ-------------*/
		temp = USART_ReceiveData(USART2);
		//USART_SendData(USART1, temp);
		if (temp == 'a')		mode_flag = 1;
		else if (temp == 'b')	mode_flag = 2;	//������Ӧģʽ(�ֻ�ң��APP����ʵ��)
		else if (temp == 'c')	mode_flag = 3;
		else if (temp == 'd')	mode_flag = 4;
		else if (temp == 'e')	mode_flag = 5;
		else if (temp == 'f')	mode_flag = 6;
		else if (temp == 'g')	mode_flag = 7;

		/*------------��־λ�Ĵ���-------------*/
		if (t == 0 && mode_flag == 4)	//���ˣ�ȫ�֣�����һ��mode_flag=4
		{
			mode_flag = 0;				//����ͨ������mode_flag��־λ��ʵ��һ�������ˡ���Ч��
			t = 1;						//ͬʱ�ƴ������(���治��0?)
		}

		/*------------����ң��ģʽ�µ�ң������-------------*/
		if (mode_flag != 2)				//��ģʽ��Ϊ������Ӧģʽʱ(��ң��ģʽ)
		{
			if (temp == 'L')			//���յ�֡ͷΪL��һ֡����
			{
				Lx_flag = 1;			//��ҡ�˱�־λ ��1
			}
			if (Lx_flag == 1)			//��ʼ������һ֡����
			{
				temp_buf1[i] = temp;	//��ǰ�����ַ� ���浽����ĵ�i��Ԫ�� ����жϽ���һ����֡����
				i++;
				if (temp == '*')		//֡βΪ*ʱһ֡���ݽ������
				{
					if (n1 == 0)		//���ˣ�ȫ�֣�����һ��ҡ�����ݣ�������ջ�������
					{
						memset(Lx_Buf, 0, 10);		//���Lx_Buf������
						memset(temp_buf1, 0, 10);	//���temp_buf1������
						n1 = 1;						//ͬʱ�ƴ������(���治��0?)
					}
					strcpy(Lx_Buf, temp_buf1);		//�ѽ��յ���һ֡���ݼ���õ�֡ ���帳��Rx_Buf����
					Lx_flag = 0;					//��ҡ�����ݽ�����ϣ���ձ�־λ
					i = 0;							//ͬʱ������������
				}
			}

			if (temp == 'R')			//���յ�֡ͷΪR��һ֡����
			{
				Rx_flag = 1;
			}
			if (Rx_flag == 1)			//��ʼ������һ֡����
			{
				temp_buf2[j] = temp;	//��ǰ�����ַ� ���浽����ĵ�i��Ԫ�� ����жϽ���һ����֡����
				j++;
				if (temp == '*')		//֡βΪ*ʱһ֡���ݽ������
				{
					if (n2 == 0)		//���ˣ�ȫ�֣�����һ��ҡ�����ݣ�������ջ�������
					{
						memset(Rx_Buf, 0, 10);		//���Rx_Buf������
						memset(temp_buf2, 0, 10);	//���temp_buf2������
						n2 = 1;						//ͬʱ�ƴ������(���治��0?)
					}
					strcpy(Rx_Buf, temp_buf2);		//�ѽ��յ���һ֡���ݼ���õ�֡ ���帳��Rx_Buf����
					Rx_flag = 0;					//��ҡ�����ݽ�����ϣ���ձ�־λ
					j = 0;							//ͬʱ������������
				}
			}
		}
		else	//mode_flag == 2	�� ��ǰģʽΪ������Ӧģʽ
		{
			if (temp == 'A')	//���յ�֡ͷΪA��һ֡����(������Ӧģʽ����)
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

int fputc(int ch, FILE* f)
{
	USART_SendData(USART1, (uint8_t)ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);	//���ͻ�����Ϊ��->����
	return (ch);
}

int fgetc(FILE* f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != SET);//���ջ������ǿ�->�յ�
	return ((int)USART_ReceiveData(USART1));
}


