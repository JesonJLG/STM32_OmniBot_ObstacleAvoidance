#include "iic.h"


/**************************************************
�������ƣ�IIC_Init(void)
�������ܣ�IICʼ������
��ڲ�������
���ز�������
***************************************************/
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����һ��GPIO_InitTypeDef���͵Ľṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//����GPIO�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;   //PB6,PB7
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //ͨ���������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//��������50MHZ
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);     //PB6,PB7�����
}

/**************************************************
�������ƣ�SDA_Out(void)
�������ܣ�SDA�������
��ڲ�������
���ز�������
***************************************************/
static void SDA_Out(void)
{
	GPIO_InitTypeDef GPIO_Struct;
	
	GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Struct.GPIO_Pin = GPIO_Pin_7;//SDA
	GPIO_Init(GPIOB,&GPIO_Struct);
}

/**************************************************
�������ƣ�SDA_In(void)
�������ܣ�SDA���뺯��
��ڲ�������
���ز�������
***************************************************/
static void SDA_In(void)
{
	GPIO_InitTypeDef GPIO_Struct;
	
	GPIO_Struct.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Struct.GPIO_Pin = GPIO_Pin_7;  //SDA
	GPIO_Init(GPIOB,&GPIO_Struct);
}


/**************************************************
�������ƣ�IIC_Start(void)
�������ܣ���ʼ�źţ���SCLΪ���ڼ䣬SDA�ɸߵ��͵�����
��ڲ�������
���ز�������
***************************************************/
void IIC_Start(void)
{
	SDA_Out();      //   SCL _________________
	SCL_UP;         // 												\
	SDA_UP;         //                         \___________
	delay_ms(4);    //
	SDA_DOWN;       //               _____ |<-->| >4us
	delay_ms(4);    //              /     \
	SCL_DOWN;       //   SDA ______/       \_______________
	delay_ms(2);    //
}


/**************************************************
�������ƣ�IIC_Stop(void)
�������ܣ�ֹͣ�źţ���SCLΪ���ڼ䣬SDA�ɵ͵��ߵ�����
��ڲ�������
���ز�������
***************************************************/
void IIC_Stop(void)
{
	SDA_Out();      //               ______________________
	SCL_DOWN;       //              /
	SDA_DOWN;       //   SCL ______/
	delay_ms(4);    //
	SCL_UP;         //         >4us |<-->|  _____
	delay_ms(4);    //                     /     \
	SDA_UP;         //   SDA _____________/       \________
	delay_ms(2);    //
}


/**************************************************
�������ƣ�IIC_Wait_Ack(void)
�������ܣ��ȴ�Ӧ���źŵ���
��ڲ�������
���ز�����1������Ӧ��ʧ��
          0������Ӧ��ɹ�
***************************************************/
u8 IIC_Wait_Ack(void)
{
	u8 i=0;          
	SDA_In();        
	SCL_DOWN;        
//	SDA_UP;       
	delay_ms(2);    
	SCL_UP;         
	delay_ms(2);     
	if(SDA_READ)    
	{                
		i = 1;           
	}                           
	else            
	{                
		i = 0;         
	}
	SCL_DOWN;
	delay_ms(2);
	return i;
}

/********************************************************************
�������ƣ�IIC_Ack(void)
�������ܣ�����Ӧ�𣬽������ڵ�9��ʱ������֮ǰ�ĵ͵�ƽ�ڼ佫SDA������
					����ȷ���ڸ�ʱ�ӵĸߵ�ƽ�ڼ�Ϊ�ȶ��ص͵�ƽ
��ڲ�������
���ز�������
********************************************************************/
void IIC_Ack(void)
{
	SDA_Out();         //         >4us |<-->|           Ӧ��0
	SCL_DOWN;          //               _____
	SDA_DOWN;          //              /     \
	delay_ms(2);       //  SCL _______/       \______________
	SCL_UP;            //
	delay_ms(2);       //  SDA ____                __________
	SCL_DOWN;          //          \              / 
	delay_ms(2);       //           \____________/
	SDA_UP;
	delay_ms(2);
}


/********************************************************************
�������ƣ�IIC_Nack(void)
�������ܣ�������Ӧ��
��ڲ�������
���ز�������
********************************************************************/
void IIC_Nack(void)
{
	SDA_Out();				 //
	SCL_DOWN;          //              _____
	SDA_DOWN;          //             /     \
	delay_ms(2);       // SCL _______/       \_______________
	SDA_UP;            //
	delay_ms(2);       //           ____________   
	SCL_UP;            //          /            \ 
	delay_ms(2);       // SDA ____/              \___________
	SCL_DOWN;          //
	delay_ms(2);       //
}


/********************************************************************
�������ƣ�IIC_Send_Byte(u8 Byte)
�������ܣ�����һ���ֽ�
��ڲ�������
���ز�������
********************************************************************/
void IIC_Send_Byte(u8 Byte)
{
	u8 i = 0;
	SDA_Out();
	for(i=0;i<8;i++)
	{
		if(Byte&0x80)
		{
				SDA_UP;
		}
		else
		{
				SDA_DOWN;
		}
		delay_ms(2);
		SCL_UP;
		delay_ms(2);
		SCL_DOWN;
		Byte <<= 1;
		delay_ms(2);
	}
	SDA_UP;
}

/********************************************************************
�������ƣ�IIC_Read_Byte(void)
�������ܣ���һ���ֽ�
��ڲ�������
���ز�������
********************************************************************/
u8 IIC_Read_Byte(void)
{
	u8 i = 0;
	u8 Byte = 0;
	SDA_In();
	for(i=0;i<8;i++)
	{
		Byte <<= 1;
		SCL_UP;
		delay_ms(2);
		if(SDA_READ)
		{
			Byte |= 0x01; 
		}
		else
		{
			Byte |= 0x00;
		}
		SCL_DOWN;
		delay_ms(2);
	}
	return Byte;
}


