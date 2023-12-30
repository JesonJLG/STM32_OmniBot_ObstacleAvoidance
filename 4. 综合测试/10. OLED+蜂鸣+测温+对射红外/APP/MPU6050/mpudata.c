#include "mpudata.h"
#include "joystick.h"
#include "nrf24l01.h"
#include "oled.h"
#include "stdio.h" //��׼���������

char tmp_buf[33];			//�ַ�������
float pitch,roll,yaw; 		//ŷ����:�����ǣ�ƫ���ǣ���ת��
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����  angular acceleration
short gyrox,gyroy,gyroz;	//������ԭʼ����  gyroscope
int mpu_x_axis = 0, mpu_y_axis = 0;

unsigned char mpu_data[12] = {0};  //�ұ�ҡ���ַ�����

void MPUInit(void)
{
	MPU_Init();	              //��ʼ��MPU6050
	while(mpu_dmp_init());    //��ʼ��mpu_dmp��
}

void MPU_Read(void)
{	
	int temp = 0;
	if(mpu_dmp_get_data(&pitch,&roll,&yaw) == 0)//dmp����õ����ݣ��Է���ֵ�����ж�
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
		
		temp=pitch*10;							 //��tempΪpitch
		if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
		{
			temp=-temp;	 
		  sprintf((char *)tmp_buf," -%d.%d",temp/10,temp%10);
		  OLED_ShowString(0,7,(u8 *)tmp_buf,8);			//�Ը�����ȡ��		
		}
		else                                    //�ж�Ϊ��ʱ 
		{
			sprintf((char *)tmp_buf,"  %d.%d",temp/10,temp%10);
		   OLED_ShowString(0,7,(u8 *)tmp_buf,8);
		}	
		
		temp=roll*10;                            //��tempΪroll
		if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
		{
			temp=-temp;
			sprintf((char *)tmp_buf," -%d.%d",temp/10,temp%10);
		  OLED_ShowString(90,7,(u8 *)tmp_buf,8);						    //�Ը�����ȡ��	
		}
		else                                    //�ж�Ϊ��ʱ
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
	if(mpu_x_axis < 10)  //�ұ�ҡ��XֵΪ��λ��ʱ
	{
		mpu_data[2] = GetChar(mpu_x_axis);
		mpu_data[3] = 'Y';
		
	  if(mpu_y_axis < 10)  //�ұ�ҡ��YֵΪ��λ��ʱ
		{
		  mpu_data[4] = '0';
			mpu_data[5] = '*';
		}
		
		if(mpu_y_axis < 100 && mpu_y_axis >= 10)  //�ұ�ҡ��YֵΪ����λ��ʱ
	  {
			mpu_data[4] = GetChar(mpu_y_axis / 10);
			mpu_data[5] = GetChar(mpu_y_axis % 10);
			mpu_data[6] = '*';
	  }
		
		if(mpu_y_axis < 1000 && mpu_y_axis >= 100)  //�ұ�ҡ��YֵΪ��λ��ʱ
	  {
			mpu_data[4] = GetChar(mpu_y_axis / 100);
			mpu_data[5] = GetChar(mpu_y_axis % 100 / 10);
			mpu_data[6] = GetChar(mpu_y_axis % 10);
			mpu_data[7] = '*';
	  }
	}
	
	if(mpu_x_axis < 100 && mpu_x_axis >= 10)  //�ұ�ҡ��XֵΪ����λ��ʱ
	{
		mpu_data[2] = GetChar(mpu_x_axis / 10);
		mpu_data[3] = GetChar(mpu_x_axis % 10);
		mpu_data[4] = 'Y';
		
	  if(mpu_y_axis < 10)  //�ұ�ҡ��YֵΪ��λ��ʱ
		{
			mpu_data[5] = '0';
			mpu_data[6] = '*';
		}
		
		if(mpu_y_axis < 100 && mpu_y_axis >= 10)  //�ұ�ҡ��YֵΪ����λ��ʱ
	  {
			mpu_data[5] = GetChar(mpu_y_axis / 10);
			mpu_data[6] = GetChar(mpu_y_axis % 10);
			mpu_data[7] = '*';
	  }
		
		if(mpu_y_axis < 1000 && mpu_y_axis >= 100)  //�ұ�ҡ��YֵΪ��λ��ʱ
	  {
			mpu_data[5] = GetChar(mpu_y_axis / 100);
			mpu_data[6] = GetChar(mpu_y_axis % 100 / 10);
			mpu_data[7] = GetChar(mpu_y_axis % 10);
			mpu_data[8] = '*';
	  }	
	}
	
	if(mpu_x_axis < 1000 && mpu_x_axis >= 100)  //�ұ�ҡ��XֵΪ��λ��ʱ
	{
		mpu_data[2] = GetChar(mpu_x_axis / 100);
		mpu_data[3] = GetChar(mpu_x_axis % 100 / 10);
		mpu_data[4] = GetChar(mpu_x_axis % 10);
		mpu_data[5] = 'Y';
		
	  if(mpu_y_axis < 10)  //�ұ�ҡ��YֵΪ��λ��ʱ
		{
			mpu_data[6] = '0';
			mpu_data[7] = '*';
		}
		
		if(mpu_y_axis < 100 && mpu_y_axis >= 10)  //�ұ�ҡ��YֵΪ����λ��ʱ
	  {
			mpu_data[6] = GetChar(mpu_y_axis / 10);
			mpu_data[7] = GetChar(mpu_y_axis % 10);
			mpu_data[8] = '*';
	  }
		
		if(mpu_y_axis < 1000 && mpu_y_axis >= 100)  //�ұ�ҡ��YֵΪ��λ��ʱ
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
* �� �� ��         : SendJoystickData
* ��������		     : ��ҡ�˵����귢�ͳ�ȥ
* ��    ��         : �� 
* ��    ��         : ��
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

