#include "mpudata.h"
#include "joystick.h"
#include "nrf24l01.h"
#include "oled.h"
#include "stdio.h" //标准输入输出库

char tmp_buf[33];			//字符串数组
float pitch,roll,yaw; 		//欧拉角:俯仰角，偏航角，滚转角
short aacx,aacy,aacz;		//加速度传感器原始数据  angular acceleration
short gyrox,gyroy,gyroz;	//陀螺仪原始数据  gyroscope
int mpu_x_axis = 0, mpu_y_axis = 0;

unsigned char mpu_data[12] = {0};  //右边摇杆字符坐标

void MPUInit(void)
{
	MPU_Init();	              //初始化MPU6050
	while(mpu_dmp_init());    //初始化mpu_dmp库
}

void MPU_Read(void)
{	
	int temp = 0;
	if(mpu_dmp_get_data(&pitch,&roll,&yaw) == 0)//dmp处理得到数据，对返回值进行判断
	{ 	
		mpu_y_axis =pitch*10;
		if(mpu_y_axis > 400)
			mpu_y_axis = 400;

		if(mpu_y_axis < -400)
			mpu_y_axis = -400;

		mpu_y_axis = Map(mpu_y_axis, -400, 400, 0, 255);
		
		//my_printf2("X:%d  ", mpu_y_axis);
		
		mpu_x_axis=-(roll*10); 
		if(mpu_x_axis > 400)
			mpu_x_axis = 400;

		if(mpu_x_axis < -400)
			mpu_x_axis = -400;
		
		mpu_x_axis = Map(mpu_x_axis, -400, 400, 0, 255);
		//my_printf2("Y:%d\n", mpu_x_axis);
		
		temp=pitch*10;							 //赋temp为pitch
		if(temp<0)								//对数据正负判断，判断为负时
		{
			temp=-temp;	 
		  sprintf((char *)tmp_buf," -%d.%d",temp/10,temp%10);
		  OLED_ShowString(0,7,(u8 *)tmp_buf,8);			//对负数据取反		
		}
		else                                    //判断为正时 
		{
			sprintf((char *)tmp_buf,"  %d.%d",temp/10,temp%10);
		   OLED_ShowString(0,7,(u8 *)tmp_buf,8);
		}	
		
		temp=roll*10;                            //赋temp为roll
		if(temp<0)								//对数据正负判断，判断为负时
		{
			temp=-temp;
			sprintf((char *)tmp_buf," -%d.%d",temp/10,temp%10);
		  OLED_ShowString(90,7,(u8 *)tmp_buf,8);						    //对负数据取反	
		}
		else                                    //判断为正时
		{
			sprintf((char *)tmp_buf,"  %d.%d",temp/10,temp%10);
		  OLED_ShowString(90,7,(u8 *)tmp_buf,8);
		}
	}
}

void MpuDataProcessing(void)	
{
	MPU_Read();
	memset(mpu_data,0,12);
	
	mpu_data[0] = 'R';
	mpu_data[1] = 'X';
	if(mpu_x_axis < 10)  //右边摇杆X值为个位数时
	{
		mpu_data[2] = GetChar(mpu_x_axis);
		mpu_data[3] = 'Y';
		
	  if(mpu_y_axis < 10)  //右边摇杆Y值为个位数时
		{
		  mpu_data[4] = '0';
			mpu_data[5] = '*';
		}
		
		if(mpu_y_axis < 100 && mpu_y_axis >= 10)  //右边摇杆Y值为个两位数时
	  {
			mpu_data[4] = GetChar(mpu_y_axis / 10);
			mpu_data[5] = GetChar(mpu_y_axis % 10);
			mpu_data[6] = '*';
	  }
		
		if(mpu_y_axis < 1000 && mpu_y_axis >= 100)  //右边摇杆Y值为三位数时
	  {
			mpu_data[4] = GetChar(mpu_y_axis / 100);
			mpu_data[5] = GetChar(mpu_y_axis % 100 / 10);
			mpu_data[6] = GetChar(mpu_y_axis % 10);
			mpu_data[7] = '*';
	  }
	}
	
	if(mpu_x_axis < 100 && mpu_x_axis >= 10)  //右边摇杆X值为个两位数时
	{
		mpu_data[2] = GetChar(mpu_x_axis / 10);
		mpu_data[3] = GetChar(mpu_x_axis % 10);
		mpu_data[4] = 'Y';
		
	  if(mpu_y_axis < 10)  //右边摇杆Y值为个位数时
		{
			mpu_data[5] = '0';
			mpu_data[6] = '*';
		}
		
		if(mpu_y_axis < 100 && mpu_y_axis >= 10)  //右边摇杆Y值为个两位数时
	  {
			mpu_data[5] = GetChar(mpu_y_axis / 10);
			mpu_data[6] = GetChar(mpu_y_axis % 10);
			mpu_data[7] = '*';
	  }
		
		if(mpu_y_axis < 1000 && mpu_y_axis >= 100)  //右边摇杆Y值为三位数时
	  {
			mpu_data[5] = GetChar(mpu_y_axis / 100);
			mpu_data[6] = GetChar(mpu_y_axis % 100 / 10);
			mpu_data[7] = GetChar(mpu_y_axis % 10);
			mpu_data[8] = '*';
	  }	
	}
	
	if(mpu_x_axis < 1000 && mpu_x_axis >= 100)  //右边摇杆X值为三位数时
	{
		mpu_data[2] = GetChar(mpu_x_axis / 100);
		mpu_data[3] = GetChar(mpu_x_axis % 100 / 10);
		mpu_data[4] = GetChar(mpu_x_axis % 10);
		mpu_data[5] = 'Y';
		
	  if(mpu_y_axis < 10)  //右边摇杆Y值为个位数时
		{
			mpu_data[6] = '0';
			mpu_data[7] = '*';
		}
		
		if(mpu_y_axis < 100 && mpu_y_axis >= 10)  //右边摇杆Y值为个两位数时
	  {
			mpu_data[6] = GetChar(mpu_y_axis / 10);
			mpu_data[7] = GetChar(mpu_y_axis % 10);
			mpu_data[8] = '*';
	  }
		
		if(mpu_y_axis < 1000 && mpu_y_axis >= 100)  //右边摇杆Y值为三位数时
	  {
			mpu_data[6] = GetChar(mpu_y_axis / 100);
			mpu_data[7] = GetChar(mpu_y_axis % 100 / 10);
			mpu_data[8] = GetChar(mpu_y_axis % 10);
			mpu_data[9] = '*';
	  }	
	}
	
	//my_printf2("%s\n", mpu_data);
}

/*******************************************************************************
* 函 数 名         : SendJoystickData
* 函数功能		     : 将摇杆的坐标发送出去
* 输    入         : 无 
* 输    出         : 无
*******************************************************************************/
void SendMpuData(void)
{
	MpuDataProcessing();
		
	NRF24L01_TX_Mode();
	if(NRF24L01_TxPacket(mpu_data)==TX_OK)
	{
		//my_printf2("TX:Success\n");
		delay_ms(1);	
	}
	else
	{
		//my_printf2("TX:Error\n");
	}
}

