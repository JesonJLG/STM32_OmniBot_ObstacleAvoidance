#include "sys.h"
#include "usart.h"
#include "string.h"

//ע��:ʹ������ģ��ʱ������ʹ��9600,���ܳ���9600������

u8 flag=0;
u8 mode_flag=0;

//char Lx_Buf[10]={0};


char Lx_Buf[10]= {0}; //��ҡ�����ݽ��ջ�����
char Rx_Buf[10]= {0}; //��ҡ�����ݽ��ջ�����
char Pitch_Roll_Buf[20];//APPƫ�������ݽ��ջ�����


void USART_SendByte(USART_TypeDef* USARTx,uint16_t Data)//����һ���ֽ�
{
    USARTx->DR=(Data & (uint16_t)0x01FF);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET);
}


void USART1_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

    //USART1_RX	  GPIOA.10��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10

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
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

    //USART2_TX   GPIOA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART2_RX	  GPIOA.3��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //Usart2 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;								 //���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;		 //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;        //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure);     //��ʼ������2
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���2
}


void USART3_Init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

    //USART3_TX   GPIOB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //USART3_RX	  GPIOB.11��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //Usart3 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &USART_InitStructure);     //��ʼ������3
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3

}

void USART2_IRQHandler(void)	//����ң�ش���
{
    u8 temp;
    static u8 t=0,n1=0,n2=0;
    static u8 i=0,j=0,k=0;
    static char temp_buf1[10]= {0},temp_buf2[10]= {0},temp_buf3[20]= {0};
    static u8 Lx_flag=0,Rx_flag=0,Pr_flag=0;//����ҡ�ˣ������ǽ������ݱ�־λ

    if(USART_GetITStatus(USART2, USART_IT_RXNE)!=RESET)	//���յ�����
    {
        temp=USART_ReceiveData(USART2);
		USART_SendData(USART1, temp);
        if(temp=='a')			mode_flag=1;
        else if(temp=='b')		mode_flag=2;
        else if(temp=='c')		mode_flag=3;
        else if(temp=='d')		mode_flag=4;
        else if(temp=='e')		mode_flag=5;
        else if(temp=='f')		mode_flag=6;
        else if(temp=='g')		mode_flag=7;

        if(t==0&&mode_flag==4)//���˵�һ��mode_flag=4
        {
            mode_flag=0;
            t=1;
        }

        if(mode_flag!=2)//��ģʽ��Ϊ������Ӧģʽʱ(ң��ģʽ)
        {
            if(temp=='L'&&temp!='a'&&temp!='b'&&temp!='c'&&temp!='d'&&temp!='e'&&temp!='f')//���յ�֡ͷΪL��һ֡����
            {
                Lx_flag=1;//��־λ��1
            }
            if(Lx_flag==1&&temp!='a'&&temp!='b'&&temp!='c'&&temp!='d'&&temp!='e'&&temp!='f')//��ʼ������һ֡����
            {
                temp_buf1[i]=temp;
                i++;
                if(temp=='*')//֡βΪ*ʱһ֡���ݽ������
                {
                    if(n1==0)//���˵�һ��ҡ������
                    {
                        memset(Lx_Buf,0,10);
                        memset(temp_buf1,0,10);
                        n1=1;
                    }
                    strcpy(Lx_Buf,temp_buf1);
                    Lx_flag=0;
                    i=0;
                }
            }

            if(temp=='R'&&temp!='a'&&temp!='b'&&temp!='c'&&temp!='d'&&temp!='e'&&temp!='f')//���յ�֡ͷΪR��һ֡����
            {
                Rx_flag=1;
            }
            if(Rx_flag==1&&temp!='a'&&temp!='b'&&temp!='c'&&temp!='d'&&temp!='e'&&temp!='f')//��ʼ������һ֡����
            {
                temp_buf2[j]=temp;
                j++;
                if(temp=='*')//֡βΪ*ʱһ֡���ݽ������
                {
                    if(n2==0)//���˵�һ��ҡ������
                    {
                        memset(Rx_Buf,0,10);
                        memset(temp_buf2,0,10);
                        n2=1;
                    }
                    strcpy(Rx_Buf,temp_buf2);
                    Rx_flag=0;
                    j=0;
                }
            }
        }
        else
        {
            if(temp=='A'&&temp!='a'&&temp!='b'&&temp!='c'&&temp!='d'&&temp!='e'&&temp!='f')//���յ�֡ͷΪA��һ֡����(������Ӧģʽ����)
            {
                Pr_flag=1;
                memset(Pitch_Roll_Buf,0,20);
                memset(temp_buf3,0,20);
            }
            if(Pr_flag==1&&temp!='a'&&temp!='b'&&temp!='c'&&temp!='d'&&temp!='e'&&temp!='f')
            {
                temp_buf3[k]=temp;
                k++;
                if(temp=='*')
                {
                    strcpy(Pitch_Roll_Buf,temp_buf3);
                    Pr_flag=0;
                    k=0;
                }
            }
        }
    }
}


void USART3_IRQHandler(void)
{
    u8 temp;
    if(USART_GetITStatus(USART2, USART_IT_RXNE)!=RESET)
    {
        temp=USART_ReceiveData(USART2);
        switch(temp)
        {
        case 'a':
            flag=1;
            break;
        case 'b':
            flag=2;
            break;
        }
    }
    USART_SendByte(USART2,temp);
}



/*************************���ڷ��ͺ���*************************************/

void USART1_Send_Byte(u8 Data) {
    USART_SendData(USART1, Data);
    return;
}

void USART1_Send_nByte(u8 *Data, u16 size) {
    u16 i = 0;
    for(i=0; i<size; i++) {
        USART_SendData(USART1, Data[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
    return;
}

void USART1_Send_Str(u8 *Data) {
    while(*Data) {
        USART_SendData(USART1, *Data++);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
    return;
}

void USART2_Send_Byte(u8 Data) {
    USART_SendData(USART2, Data);
    return;
}

void USART2_Send_nByte(u8 *Data, u16 size) {
    u16 i = 0;
    for(i=0; i<size; i++) {
        USART_SendData(USART2, Data[i]);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
    return;
}

void USART2_Send_Str(u8 *Data) {
    while(*Data) {
        USART_SendData(USART2, *Data++);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
    return;
}

void USART3_Send_Byte(u8 Data) {
    USART_SendData(USART3, Data);
    return;
}

void USART3_Send_nByte(u8 *Data, u16 size) {
    u16 i = 0;
    for(i=0; i<size; i++) {
        USART_SendData(USART3, Data[i]);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    }
    return;
}

void USART3_Send_Str(u8 *Data) {
    while(*Data) {
        USART_SendData(USART3, *Data++);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    }
    return;
}


int fputc(int ch,FILE *f)
{
    USART_SendData(USART1,(uint8_t) ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);	//���ͻ�����Ϊ��-������
    return (ch);
}

int fgetc(FILE *f)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != SET);	//���ջ������ǿ�-���յ�
    return ((int)USART_ReceiveData(USART1));
}


