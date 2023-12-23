#include "iic.h"


/**************************************************
函数名称：IIC_Init(void)
函数功能：IIC始化函数
入口参数：无
返回参数：无
***************************************************/
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义一个GPIO_InitTypeDef类型的结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//开启GPIO端口时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;   //PB6,PB7
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //通用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//引脚速率50MHZ
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);     //PB6,PB7输出高
}

/**************************************************
函数名称：SDA_Out(void)
函数功能：SDA输出函数
入口参数：无
返回参数：无
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
函数名称：SDA_In(void)
函数功能：SDA输入函数
入口参数：无
返回参数：无
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
函数名称：IIC_Start(void)
函数功能：起始信号，当SCL为高期间，SDA由高到低的跳变
入口参数：无
返回参数：无
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
函数名称：IIC_Stop(void)
函数功能：停止信号，当SCL为高期间，SDA由低到高的跳变
入口参数：无
返回参数：无
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
函数名称：IIC_Wait_Ack(void)
函数功能：等待应答信号到来
入口参数：无
返回参数：1，接收应答失败
          0，接收应答成功
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
函数名称：IIC_Ack(void)
函数功能：产生应答，接收器在第9个时钟脉冲之前的低电平期间将SDA线拉低
					并且确保在该时钟的高电平期间为稳定地低电平
入口参数：无
返回参数：无
********************************************************************/
void IIC_Ack(void)
{
	SDA_Out();         //         >4us |<-->|           应答0
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
函数名称：IIC_Nack(void)
函数功能：不产生应答
入口参数：无
返回参数：无
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
函数名称：IIC_Send_Byte(u8 Byte)
函数功能：发送一个字节
入口参数：无
返回参数：无
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
函数名称：IIC_Read_Byte(void)
函数功能：读一个字节
入口参数：无
返回参数：无
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


