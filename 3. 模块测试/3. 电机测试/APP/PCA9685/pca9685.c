#include "pca9685.h"
#include "delay.h"
#include "math.h"


//��ʼ��IIC
void PCA_IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 �����
}


//����IIC��ʼ�ź�
void PCA_IIC_Start(void)
{
	PCA_SDA_OUT();     //sda�����
	PCA_IIC_SDA=1;	  	  
	PCA_IIC_SCL=1;
	delay_us(4);
 	PCA_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	PCA_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	


//����IICֹͣ�ź�
void PCA_IIC_Stop(void)
{
	PCA_SDA_OUT();//sda�����
	PCA_IIC_SCL=0;
	PCA_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	PCA_IIC_SCL=1; 
	PCA_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}


//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 PCA_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	PCA_SDA_IN();      //SDA����Ϊ����  
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
	PCA_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 


//����ACKӦ��
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


//������ACKӦ��		    
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


//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void PCA_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		PCA_SDA_OUT(); 	    
    PCA_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			PCA_IIC_SDA=1;
		else
			PCA_IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		PCA_IIC_SCL=1;
		delay_us(2); 
		PCA_IIC_SCL=0;	
		delay_us(2);
    }	 
} 


//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 PCA_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	PCA_SDA_IN();//SDA����Ϊ����
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
        PCA_IIC_NAck();//����nACK
    else
        PCA_IIC_Ack(); //����ACK   
    return receive;
}


void PCA_write(u8 adrr,u8 data)//��PCAд����,adrrd��ַ,data����
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

u8 PCA_read(u8 adrr)//��PCA������
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


void PCA_setfreq(float freq)//����PWMƵ��
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


/*num:���PWM�������0~15��on:PWM��������ֵ0~4096,off:PWM�½�����ֵ0~4096
һ��PWM���ڷֳ�4096�ݣ���0��ʼ+1�������Ƶ�onʱ����Ϊ�ߵ�ƽ������������offʱ
����Ϊ�͵�ƽ��ֱ������4096���¿�ʼ�����Ե�on������0ʱ������ʱ,��on����0ʱ��
off/4096��ֵ����PWM��ռ�ձȡ�*/
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
�������ƣ�PCA_Init(float hz)
�������ܣ�����Ƕȳ�ʼ��
��ڲ�����hz:Ƶ��
					angle:��ʼ���Ƕ�
���ز�������
***************************************************************************/
void PCA_Init(float hz,u8 angle)
{
	PCA_IIC_Init();
	PCA_write(pca_mode1,0x0);
	PCA_setfreq(hz);//����PWMƵ��
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
�������ƣ�PCA_MG9XX(u8 num,u8 start_angle,u8 end_angle,u8 speed)
�������ܣ����ƶ��ת����
��ڲ�����num:��ѡ0~15
					start_angle:��ѡ0~180
					end_angle:��ѡ0~180
					speed:������� 0 ������ֵ���� 1 ʱ�ٶ���죬��ֵԽ���ٶ�ԽС
���ز�������
����˵����ע�����1������ٶ�С��10������ٶȴ�
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
�������ƣ�PCA_Text(void)
�������ܣ�PCA9685���ת������
��ڲ�������
���ز�������
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

