#ifndef __PCA_H
#define __PCA_H	
#include "stm32f10x.h"

#define pca_adrr 0x80

#define pca_mode1 0x0
#define pca_pre 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define jdMIN  115 // minimum
#define jdMAX  590 // maximum
#define jd000  130 //0度对应4096的脉宽计数值
#define jd180  520 //180度对应4096的脉宽计算值

//IO方向设置
 
#define PCA_SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define PCA_SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO操作函数	 /898
#define PCA_IIC_SCL    PBout(6) //SCL
#define PCA_IIC_SDA    PBout(7) //SDA	 
#define PCA_READ_SDA   PBin(7)  //输入SDA 

//IIC所有操作函数
void PCA_IIC_Init(void);                //初始化IIC的IO口				 
void PCA_IIC_Start(void);				//发送IIC开始信号
void PCA_IIC_Stop(void);	  			//发送IIC停止信号
void PCA_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 PCA_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 PCA_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void PCA_IIC_Ack(void);					//IIC发送ACK信号
void PCA_IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  

void PCA_write(u8 adrr,u8 data);
u8 PCA_read(u8 adrr);
void PCA_Init(float hz,u8 angle);
void PCA_setfreq(float freq);
void PCA_setpwm(u8 num, u32 on, u32 off);
void PCA_MG9XX(u8 num,u8 start_angle,u8 end_angle,u8 speed);
u16 calculate_PWM(u8 angle);

void PCA_Text(void);

#endif
