#include "pca9685.h"
#include "delay.h"
#include "math.h"


//初始化IIC
void PCA_IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 输出高
}


//产生IIC起始信号
void PCA_IIC_Start(void)
{
	PCA_SDA_OUT();     //sda线输出
	PCA_IIC_SDA=1;	  	  
	PCA_IIC_SCL=1;
	delay_us(4);
 	PCA_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	PCA_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	


//产生IIC停止信号
void PCA_IIC_Stop(void)
{
	PCA_SDA_OUT();//sda线输出
	PCA_IIC_SCL=0;
	PCA_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	PCA_IIC_SCL=1; 
	PCA_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}


//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 PCA_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	PCA_SDA_IN();      //SDA设置为输入  
	PCA_IIC_SDA=1;delay_us(1);	   
	PCA_IIC_SCL=1;delay_us(1);	 
	while(PCA_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			PCA_IIC_Stop();
			return 1;
		}
	}
	PCA_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 


//产生ACK应答
void PCA_IIC_Ack(void)
{
	PCA_IIC_SCL=0;
	PCA_SDA_OUT();
	PCA_IIC_SDA=0;
	delay_us(2);
	PCA_IIC_SCL=1;
	delay_us(2);
	PCA_IIC_SCL=0;
}


//不产生ACK应答		    
void PCA_IIC_NAck(void)
{
	PCA_IIC_SCL=0;
	PCA_SDA_OUT();
	PCA_IIC_SDA=1;
	delay_us(2);
	PCA_IIC_SCL=1;
	delay_us(2);
	PCA_IIC_SCL=0;
}	


//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void PCA_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		PCA_SDA_OUT(); 	    
    PCA_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			PCA_IIC_SDA=1;
		else
			PCA_IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		PCA_IIC_SCL=1;
		delay_us(2); 
		PCA_IIC_SCL=0;	
		delay_us(2);
    }	 
} 


//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 PCA_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	PCA_SDA_IN();//SDA设置为输入
  for(i=0;i<8;i++ )
	{
        PCA_IIC_SCL=0; 
        delay_us(2);
				PCA_IIC_SCL=1;
        receive<<=1;
        if(PCA_READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        PCA_IIC_NAck();//发送nACK
    else
        PCA_IIC_Ack(); //发送ACK   
    return receive;
}


void PCA_write(u8 adrr,u8 data)//向PCA写数据,adrrd地址,data数据
{ 
	PCA_IIC_Start();
	
	PCA_IIC_Send_Byte(pca_adrr);
	PCA_IIC_Wait_Ack();
	
	PCA_IIC_Send_Byte(adrr);
	PCA_IIC_Wait_Ack();
	
	PCA_IIC_Send_Byte(data);
	PCA_IIC_Wait_Ack();
	
	PCA_IIC_Stop();
}

u8 PCA_read(u8 adrr)//从PCA读数据
{
	u8 data;
	PCA_IIC_Start();
	
	PCA_IIC_Send_Byte(pca_adrr);
	PCA_IIC_Wait_Ack();
	
	PCA_IIC_Send_Byte(adrr);
	PCA_IIC_Wait_Ack();
	
	PCA_IIC_Start();
	
	PCA_IIC_Send_Byte(pca_adrr|0x01);
	PCA_IIC_Wait_Ack();
	
	data=PCA_IIC_Read_Byte(0);
	PCA_IIC_Stop();
	
	return data;
}


void PCA_setfreq(float freq)//设置PWM频率
{
		u8 prescale,oldmode,newmode;
		double prescaleval;
		freq *= 0.92; 
		prescaleval = 25000000;
		prescaleval /= 4096;
		prescaleval /= freq;
		prescaleval -= 1;
		prescale =floor(prescaleval + 0.5f);

		oldmode = PCA_read(pca_mode1);
	
		newmode = (oldmode&0x7F) | 0x10; // sleep
	
		PCA_write(pca_mode1, newmode); // go to sleep
	
		PCA_write(pca_pre, prescale); // set the prescaler
	
		PCA_write(pca_mode1, oldmode);
		delay_ms(2);
	
		PCA_write(pca_mode1, oldmode | 0xa1); 
}


/*num:舵机PWM输出引脚0~15，on:PWM上升计数值0~4096,off:PWM下降计数值0~4096
一个PWM周期分成4096份，由0开始+1计数，计到on时跳变为高电平，继续计数到off时
跳变为低电平，直到计满4096重新开始。所以当on不等于0时可作延时,当on等于0时，
off/4096的值就是PWM的占空比。*/
void PCA_setpwm(u8 num, u32 on, u32 off)
{
		PCA_write(LED0_ON_L+4*num,on);
		PCA_write(LED0_ON_H+4*num,on>>8);
		PCA_write(LED0_OFF_L+4*num,off);
		PCA_write(LED0_OFF_H+4*num,off>>8);
}


u16 calculate_PWM(u8 angle)
{
    return (int)(204.8*(0.5+angle*1.0/90));
}



/*************************************************************************
函数名称：PCA_Init(float hz)
函数功能：舵机角度初始化
入口参数：hz:频率
					angle:初始化角度
返回参数：无
***************************************************************************/
void PCA_Init(float hz,u8 angle)
{
	PCA_IIC_Init();
	PCA_write(pca_mode1,0x0);
	PCA_setfreq(hz);//设置PWM频率
	PCA_setpwm(0,0,calculate_PWM(angle));PCA_setpwm(1,0,calculate_PWM(angle));
	PCA_setpwm(2,0,calculate_PWM(angle));PCA_setpwm(3,0,calculate_PWM(angle));
	PCA_setpwm(4,0,calculate_PWM(angle));PCA_setpwm(5,0,calculate_PWM(angle));
	PCA_setpwm(6,0,calculate_PWM(angle));PCA_setpwm(7,0,calculate_PWM(angle));
	PCA_setpwm(8,0,calculate_PWM(angle));PCA_setpwm(9,0,calculate_PWM(angle));
	PCA_setpwm(10,0,calculate_PWM(angle));PCA_setpwm(11,0,calculate_PWM(angle));
	PCA_setpwm(12,0,calculate_PWM(angle));PCA_setpwm(13,0,calculate_PWM(angle));
	PCA_setpwm(14,0,calculate_PWM(angle));PCA_setpwm(15,0,calculate_PWM(angle));
	delay_ms(500);
}


/*************************************************************************
函数名称：PCA_MG9XX(u8 num,u8 start_angle,u8 end_angle,u8 speed)
函数功能：控制舵机转动；
入口参数：num:可选0~15
					start_angle:可选0~180
					end_angle:可选0~180
					speed:可填大于 0 的任意值，填 1 时速度最快，数值越大，速度越小
返回参数：无
函数说明：注意事项：1的最大速度小，10的最大速度大。
***************************************************************************/
void PCA_MG9XX(u8 num,u8 start_angle,u8 end_angle,u8 speed)
{
	int i,j;
	u32 off=0;
	
	switch(speed)
	{
		case'1':j=100;break;
		case'2':j=130;break;
		case'3':j=170;break;
		case'4':j=300;break;
		case'5':j=400;break;
		case'6':j=500;break;
		case'7':j=500;break;
		case'8':j=500;break;
		case'9':j=500;break;
		default:j=600;
	}
	
	if(end_angle>start_angle)
	{
		for(i=start_angle;i<=end_angle;i=i+speed)
		{
			off=(u32)(110+i*1.6);
			PCA_setpwm(num,0,off);
		}
		delay_ms(j);
	}
	if(end_angle<start_angle)
	{
		for(i=start_angle;i>=end_angle;i=i-speed)
		{
			off=(u32)(110+i*1.6);
			PCA_setpwm(num,0,off);
		}
		delay_ms(j);
	}
	if(start_angle==end_angle)
	{
			off=(u32)(110+start_angle*1.6);
			PCA_setpwm(num,0,off);
	}
}


/**************************************************
函数名称：PCA_Text(void)
函数功能：PCA9685舵机转动测试
入口参数：无
返回参数：无
***************************************************/
void PCA_Text(void)
{
	PCA_setpwm(0,0,calculate_PWM(10));PCA_setpwm(1,0,calculate_PWM(10));
	PCA_setpwm(2,0,calculate_PWM(10));PCA_setpwm(3,0,calculate_PWM(10));
	PCA_setpwm(4,0,calculate_PWM(10));PCA_setpwm(5,0,calculate_PWM(10));
	PCA_setpwm(6,0,calculate_PWM(10));PCA_setpwm(7,0,calculate_PWM(10));
	PCA_setpwm(8,0,calculate_PWM(10));PCA_setpwm(9,0,calculate_PWM(10));
	PCA_setpwm(10,0,calculate_PWM(10));PCA_setpwm(11,0,calculate_PWM(10));
	PCA_setpwm(12,0,calculate_PWM(10));PCA_setpwm(13,0,calculate_PWM(10));
	PCA_setpwm(14,0,calculate_PWM(10));PCA_setpwm(15,0,calculate_PWM(10));
	delay_ms(1500);
	
	PCA_setpwm(0,0,calculate_PWM(90));PCA_setpwm(1,0,calculate_PWM(90));
	PCA_setpwm(2,0,calculate_PWM(90));PCA_setpwm(3,0,calculate_PWM(90));
	PCA_setpwm(4,0,calculate_PWM(90));PCA_setpwm(5,0,calculate_PWM(90));
	PCA_setpwm(6,0,calculate_PWM(90));PCA_setpwm(7,0,calculate_PWM(90));
	PCA_setpwm(8,0,calculate_PWM(90));PCA_setpwm(9,0,calculate_PWM(90));
	PCA_setpwm(10,0,calculate_PWM(90));PCA_setpwm(11,0,calculate_PWM(90));
	PCA_setpwm(12,0,calculate_PWM(90));PCA_setpwm(13,0,calculate_PWM(90));
	PCA_setpwm(14,0,calculate_PWM(90));PCA_setpwm(15,0,calculate_PWM(90));
	delay_ms(1500);
	
	PCA_setpwm(0,0,calculate_PWM(170));PCA_setpwm(1,0,calculate_PWM(170));
	PCA_setpwm(2,0,calculate_PWM(170));PCA_setpwm(3,0,calculate_PWM(170));
	PCA_setpwm(4,0,calculate_PWM(170));PCA_setpwm(5,0,calculate_PWM(170));
	PCA_setpwm(6,0,calculate_PWM(170));PCA_setpwm(7,0,calculate_PWM(170));
	PCA_setpwm(8,0,calculate_PWM(170));PCA_setpwm(9,0,calculate_PWM(170));
	PCA_setpwm(10,0,calculate_PWM(170));PCA_setpwm(11,0,calculate_PWM(170));
	PCA_setpwm(12,0,calculate_PWM(170));PCA_setpwm(13,0,calculate_PWM(170));
	PCA_setpwm(14,0,calculate_PWM(170));PCA_setpwm(15,0,calculate_PWM(170));
	delay_ms(1500);
}

