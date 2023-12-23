#include "mpu6050.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"  
#include "oled.h"
#include "stdio.h"

float pitch,roll,yaw; 		//欧拉角
short aacx,aacy,aacz;		//加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据
int MPU_Dir;
u8 MPU_TxBuf[6];//陀螺仪发送数据存储区

//初始化MPU6050
//返回值:0,成功
//       其他,错误代码
u8 MPU_Init(void)
{ 
	u8 res; 
	MPU_IIC_Init();//初始化IIC总线
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
  delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
	MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
	MPU_Set_Rate(50);						//设置采样率50Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	//关闭FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG); 
	if(res==MPU_ADDR)//器件ID正确
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
		MPU_Set_Rate(50);						//设置采样率为50Hz
 	}else return 1;
	return 0;
}

//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}


//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}


//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);//设置数字低通滤波器  
}


//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}


//得到温度值
//返回值:温度值(扩大了100倍)
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


//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
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


//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
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



//IIC连续写
//addr:器件地址 
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
  MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令	
	if(MPU_IIC_Wait_Ack())	//等待应答
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
  MPU_IIC_Send_Byte(reg);	//写寄存器地址
  MPU_IIC_Wait_Ack();		//等待应答
	for(i=0;i<len;i++)
	{
		MPU_IIC_Send_Byte(buf[i]);	//发送数据
		if(MPU_IIC_Wait_Ack())		//等待ACK
		{
			MPU_IIC_Stop();	 
			return 1;		 
		}		
	}    
  MPU_IIC_Stop();	 
	return 0;	
} 



//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令	
	if(MPU_IIC_Wait_Ack())	//等待应答
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
  MPU_IIC_Send_Byte(reg);	//写寄存器地址
  MPU_IIC_Wait_Ack();		//等待应答
  MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|1);//发送器件地址+读命令	
  MPU_IIC_Wait_Ack();		//等待应答 
	while(len)
	{
		if(len==1)*buf=MPU_IIC_Read_Byte(0);//读数据,发送nACK 
		else *buf=MPU_IIC_Read_Byte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
  MPU_IIC_Stop();	//产生一个停止条件 
	return 0;	
}



//IIC写一个字节 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{ 
  MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//发送器件地址+写命令	
	if(MPU_IIC_Wait_Ack())	//等待应答
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
  MPU_IIC_Send_Byte(reg);	//写寄存器地址
  MPU_IIC_Wait_Ack();		//等待应答 
	MPU_IIC_Send_Byte(data);//发送数据
	if(MPU_IIC_Wait_Ack())	//等待ACK
	{
		MPU_IIC_Stop();	 
		return 1;		 
	}		 
  MPU_IIC_Stop();	 
	return 0;
}


//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
  MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//发送器件地址+写命令	
	MPU_IIC_Wait_Ack();		//等待应答 
  MPU_IIC_Send_Byte(reg);	//写寄存器地址
  MPU_IIC_Wait_Ack();		//等待应答
  MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);//发送器件地址+读命令	
  MPU_IIC_Wait_Ack();		//等待应答 
	res=MPU_IIC_Read_Byte(0);//读取数据,发送nACK 
  MPU_IIC_Stop();			//产生一个停止条件 
	return res;		
}


//初始化MPU6050
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

void MPU_Text(void)
{
	u8 string[10] = {0};
	MPU_Dir=10;
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
	{
		sprintf((char *)string,"Pitch:%.2f",pitch);//0300
		OLED_ShowString(16,10,string,16,1);
		sprintf((char *)string,"Roll :%.2f",roll);//0300
		OLED_ShowString(16,26,string,16,1);
		sprintf((char *)string,"Yaw  :%.2f",yaw);//0300
		OLED_ShowString(16,46,string,16,1);
		OLED_DrawLine(5,5,125,5,1);
		OLED_DrawLine(5,5,5,62,1);
		OLED_DrawLine(5,62,125,62,1);
		OLED_DrawLine(125,5,125,62,1);
		OLED_Refresh();
		delay_ms(100);
	
		printf("Pitch=%.2f\n",pitch);
		printf("Roll=%.2f\n",roll);
		if((pitch>=20&&pitch<=60)&&(roll>=-10&&roll<=10))MPU_Dir= 0;
	  if((pitch>=-60&&pitch<=-20)&&(roll>=-10&&roll<=10))MPU_Dir= 1;	
		
		if((pitch>=-10&&pitch<=10)&&(roll>=-60&&roll<=-20))MPU_Dir= 2;
		if((pitch>=-10&&pitch<=10)&&(roll>=20&&roll<=60))MPU_Dir= 3;
			
		if((pitch>=10&&pitch<=60)&&(roll>=-60&&roll<=-10))MPU_Dir= 4;
		if((pitch>=10&&pitch<=60)&&(roll>=10&&roll<=60))MPU_Dir= 5;
		if((pitch>=-60&&pitch<=-10)&&(roll>=-60&&roll<=-10))MPU_Dir= 6;
		if((pitch>=-60&&pitch<=-10)&&(roll>=10&&roll<=60))MPU_Dir= 7;
		
		if((pitch>=-90&&pitch<=-60)&&(roll>=-40&&roll<=-20))MPU_Dir= 8;
		if((pitch>=-90&&pitch<=-60)&&(roll>=20&&roll<=40))MPU_Dir= 9;
		printf("MPU_Dir=%d\n",MPU_Dir);
	}
	delay_ms(100);
}

/**************************************************
函数名称：void MPU_Data(void)
函数功能：陀螺仪数据处理函数
入口参数：无
返回参数：无
***************************************************/
void MPU_Data(void)
{
	u8 string[10] = {0};
	int Map_pitch=0,Map_roll=0;//映射后的偏航角
	
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
	{
		sprintf((char *)string,"Pitch:%.2f",pitch);//0300
		sprintf((char *)string,"Roll :%.2f",roll);//0300
		sprintf((char *)string,"Yaw  :%.2f",yaw);//0300

		if(pitch>70)pitch=70;
		if(pitch<-70)pitch=-70;
		if(roll>70)pitch=70;
		if(roll<-70)pitch=-70;
		
		Map_pitch=Map((int)pitch,-70,70,10,90);
		Map_roll=Map((int)roll,-70,70,10,90);
		
		MPU_TxBuf[0]='P';
		MPU_TxBuf[1]=Map_pitch/10+'0';
		MPU_TxBuf[2]=Map_pitch%10+'0';
		MPU_TxBuf[3]='R';
		MPU_TxBuf[4]=Map_roll/10+'0';
		MPU_TxBuf[5]=Map_roll%10+'0';
	}
	delay_ms(100);
}


/**************************************************
函数名称：void MPU_Direction(void)
函数功能：陀螺仪数据发送函数
入口参数：无
返回参数：无
***************************************************/
void MPU_Direction(void)
{
	u8 string[10] = {0};
	int Map_pitch=0,Map_roll=0;//映射后的偏航角
	
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
	{
		sprintf((char *)string,"Pitch:%.2f",pitch);//0300
		OLED_ShowString(16,10,string,16,1);
		sprintf((char *)string,"Roll :%.2f",roll);//0300
		OLED_ShowString(16,26,string,16,1);
		sprintf((char *)string,"Yaw  :%.2f",yaw);//0300
		OLED_ShowString(16,46,string,16,1);
		OLED_DrawLine(5,5,125,5,1);
		OLED_DrawLine(5,5,5,62,1);
		OLED_DrawLine(5,62,125,62,1);
		OLED_DrawLine(125,5,125,62,1);
		OLED_Refresh();
	

		if(pitch>70)pitch=70;
		if(pitch<-70)pitch=-70;
		if(roll>70)pitch=70;
		if(roll<-70)pitch=-70;
		
		Map_pitch=Map((int)pitch,-70,70,10,90);
		Map_roll=Map((int)roll,-70,70,10,90);
		
		MPU_TxBuf[0]='P';
		MPU_TxBuf[1]=Map_pitch/10+'0';
		MPU_TxBuf[2]=Map_pitch%10+'0';
		MPU_TxBuf[3]='R';
		MPU_TxBuf[4]=Map_roll/10+'0';
		MPU_TxBuf[5]=Map_roll%10+'0';
		
		NRF24L01_TX_Mode();
		if(NRF24L01_TxPacket(MPU_TxBuf)==TX_OK)
		{
			printf((char*)MPU_TxBuf);
			printf("\n");
		}
		else printf("Receive failed !\n");
	}
	
	delay_ms(100);
}



