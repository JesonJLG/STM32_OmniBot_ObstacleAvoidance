#include "mpu6050.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"  
#include "oled.h"
#include "stdio.h"

float pitch,roll,yaw; 		//ŷ����
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;	//������ԭʼ����


/*************************************************************
�������ƣ�MPU_Init(void)
�������ܣ���ʼ��MPU6050
��ڲ�������
���ز�����0,�ɹ�
					����,ʧ�� 
************************************************************/
u8 MPU_Init(void)
{ 
	u8 res; 
	MPU_IIC_Init();//��ʼ��IIC����
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//��λMPU6050
  delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//����MPU6050 
	MPU_Set_Gyro_Fsr(3);					//�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
	MPU_Set_Rate(50);						//���ò�����50Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG); 
	if(res==MPU_ADDR)//����ID��ȷ
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//���ٶ��������Ƕ�����
		MPU_Set_Rate(50);						//���ò�����Ϊ50Hz
 	}else return 1;
	return 0;
}


/*************************************************************
�������ƣ�MPU_Set_Gyro_Fsr(u8 fsr)
�������ܣ�����MPU6050�����Ǵ����������̷�Χ
��ڲ�����fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
���ز�����0,���óɹ�
					����,����ʧ�� 
************************************************************/
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ  
}


/*************************************************************
�������ƣ�MPU_Set_Accel_Fsr(u8 fsr)
�������ܣ�����MPU6050���ٶȴ����������̷�Χ
��ڲ�����fsr:0,��2g;1,��4g;2,��8g;3,��16g
���ز�����0,���óɹ�
					����,����ʧ�� 
************************************************************/
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}

/*************************************************************
�������ƣ�MPU_Set_LPF(u16 lpf)
�������ܣ�����MPU6050�����ֵ�ͨ�˲���
��ڲ�����lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
���ز�����0,���óɹ�
					����,����ʧ�� 
************************************************************/
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}

/*************************************************************
�������ƣ�MPU_Set_Rate(u16 rate)
�������ܣ�����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
��ڲ�����rate:4~1000(Hz)
���ز�����0,���óɹ�
					����,����ʧ�� 
************************************************************/
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}

/*************************************************************
�������ƣ�MPU_Get_Temperature(void)
�������ܣ��õ��¶�ֵ
��ڲ�������
���ز������¶�ֵ(������100��)
************************************************************/
short MPU_Get_Temperature(void)
{
  u8 buf[2]; 
  short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
  raw=((u16)buf[0]<<8)|buf[1];  
  temp=36.53+((double)raw)/340;  
  return temp*100;;
}

/*************************************************************
�������ƣ�MPU_Get_Gyroscope(short *gx,short *gy,short *gz)	
�������ܣ��õ�������ֵ(ԭʼֵ)
��ڲ�����gx,gy,gz:������x,y,z���ԭʼ����(������)
���ز�����0,����
					����,�������
************************************************************/
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
  u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
  return res;;
}

/*************************************************************
�������ƣ�MPU_Get_Accelerometer(short *ax,short *ay,short *az)	
�������ܣ��õ����ٶ�ֵ(ԭʼֵ)
��ڲ�����gx,gy,gz:������x,y,z���ԭʼ����(������)
���ز�����0,����
					����,�������
************************************************************/
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
  u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
  return res;;
}


/***********************************************************
�������ƣ�MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf) 		
�������ܣ�IIC����д
��ڲ�����addr:������ַ
					reg:�Ĵ�����ַ
					len:Ҫд�볤��
					buf:���ݴ洢��
���ز�����0,����
					����,�������
************************************************************/
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
  MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((addr<<1)|0);//����������ַ+д����	
	if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
  MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
  MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		MPU_IIC_Send_Byte(buf[i]);	//��������
		if(MPU_IIC_Wait_Ack())		//�ȴ�ACK
		{
			MPU_IIC_Stop();	 
			return 1;		 
		}		
	}    
  MPU_IIC_Stop();	 
	return 0;	
} 


/***********************************************************
�������ƣ�MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf) 		
�������ܣ�IIC������
��ڲ�����addr:������ַ
					reg:Ҫ��ȡ�ļĴ�����ַ
					len:Ҫ��ȡ�ĳ���
					buf:��ȡ�������ݴ洢��
���ز�����0,����
					����,�������
************************************************************/
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((addr<<1)|0);//����������ַ+д����	
	if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
  MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
  MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
  MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|1);//����������ַ+������	
  MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	while(len)
	{
		if(len==1)*buf=MPU_IIC_Read_Byte(0);//������,����nACK 
		else *buf=MPU_IIC_Read_Byte(1);		//������,����ACK  
		len--;
		buf++; 
	}    
  MPU_IIC_Stop();	//����һ��ֹͣ���� 
	return 0;	
}


/***********************************************************
�������ƣ�MPU_Write_Byte(u8 reg,u8 data) 		
�������ܣ�IICдһ���ֽ� 
��ڲ�����reg:�Ĵ�����ַ  data:����
���ز�����0,����
					����,�������
************************************************************/
u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{ 
  MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+д����	
	if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
  MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
  MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	MPU_IIC_Send_Byte(data);//��������
	if(MPU_IIC_Wait_Ack())	//�ȴ�ACK
	{
		MPU_IIC_Stop();	 
		return 1;		 
	}		 
  MPU_IIC_Stop();	 
	return 0;
}

/***********************************************************
�������ƣ�MPU_Read_Byte(u8 reg)
�������ܣ�IIC��һ���ֽ� 
��ڲ�����reg:�Ĵ�����ַ 
���ز���������������
************************************************************/
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
  MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+д����	
	MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
  MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
  MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
  MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);//����������ַ+������	
  MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	res=MPU_IIC_Read_Byte(0);//��ȡ����,����nACK 
  MPU_IIC_Stop();			//����һ��ֹͣ���� 
	return res;		
}


/***********************************************************
�������ƣ�MPU6050_Init(void)
�������ܣ���ʼ��MPU6050
��ڲ�������
���ز�������
************************************************************/
void MPU6050_Init(void)
{
	MPU_Init();
	while(MPU_Init())
	{
		OLED_ShowString(16,16,"MPU_Init failed!",16,0);
		OLED_Refresh();
		printf("MPU_Init failed!");
		printf("\n");
		printf("res=%d",MPU_Init());
		printf("\n");
		delay_ms(1000);
	}
	while(mpu_dmp_init())
	{
		OLED_ShowString(16,16,"mpu_dmp_init failed!",16,0);
		OLED_Refresh();
		printf("mpu_dmp_init failed!");
		printf("\n");
		printf("res=%d",mpu_dmp_init());
		printf("\n");
		delay_ms(1000);
	}
	printf("MPU_Init successfully!");
	printf("\n");
}


/***********************************************************
�������ƣ�MPU_Text(void)
�������ܣ�MPU6050����
��ڲ�������
���ز�������
************************************************************/
void MPU_Text(void)
{
	u8 string[10] = {0};	
	OLED_ShowString(40,0,"MPU6050",16,0);
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
	{
		sprintf((char *)string,"Pitch:%.2f",pitch);//0300
		OLED_ShowString(16,16,string,16,1);
		sprintf((char *)string,"Roll :%.2f",roll);//0300
		OLED_ShowString(16,32,string,16,1);
		sprintf((char *)string,"Yaw  :%.2f",yaw);//0300
		OLED_ShowString(16,48,string,16,1);
		OLED_Refresh();
		delay_ms(100);
	}
	OLED_Refresh();
}

