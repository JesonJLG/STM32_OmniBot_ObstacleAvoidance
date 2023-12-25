#include "control.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
#include "mymecanum.h"
#include "math.h"
#include "string.h"
#include "stdio.h"

extern u8 mode_flag;
extern char Lx_Buf[10];			//左摇杆数据接收缓冲区
extern char Rx_Buf[10];			//右摇杆数据接收缓冲区
extern char Pitch_Roll_Buf[20];	//APP偏航角数据接收缓冲区
//u8 Eva_RxBuf[4];//避障停止指令数组

/**************************************************
函数名称：Map(int val,int in_min,int in_max,int out_min,int out_max)
函数功能：映射函数
入口参数：	val:需要映射的数值
			in_min:输入值的范围最小值
			in_max:输入值的范围最大值
			out_min:输出值的范围最小值
			out_max:输出值的范围最大值

返回参数：映射后的数值
***************************************************/
int Map(int val,int in_min,int in_max,int out_min,int out_max)
{
    return (int)(val-in_min)*(out_max-out_min)/(in_max-in_min)+out_min;
}

/**************************************************
函数名称：Bluetooth_Mode(void)
函数功能：蓝牙遥控模式
入口参数：无
返回参数：无
***************************************************/
void Bluetooth_Mode(void)
{
    if(mode_flag==1)		APP_Joy_Mode();		//APP摇杆模式
    else if(mode_flag==2)	APP_Gravity_Mode();	//APP重力模式
//	else if(mode_flag==3)	Evadible_Mode();
//	else if(mode_flag==4)	Follow_Mode();
    else
    {
        printf("mode_flag:%d\r\n", mode_flag);
    }
}



/**************************************************
函数名称：APP_Joy_Mode(void)
函数功能：蓝牙模式的摇杆遥控
入口参数：无
返回参数：无
***************************************************/
void APP_Joy_Mode(void)
{
    int Joy_Lx=50, Joy_Ly = 50, Joy_Rx = 50, Joy_Ry = 50;	//middle
    int Map_Lx, Map_Ly, Map_Rx, Map_Ry;						//映射值
    int pwm1, pwm2, pwm3, pwm4;								//速度？

    if (Lx_Buf[0] == 'L')
    {   //"LX50Y50"
        Joy_Lx = (Lx_Buf[2] - '0') * 10 + (Lx_Buf[3] - '0');	//ACSII转int
        Joy_Ly = (Lx_Buf[5] - '0') * 10 + (Lx_Buf[6] - '0');
    }
    if (Rx_Buf[0] == 'R')
    {   //"RX50Y50"
        Joy_Rx = (Rx_Buf[2] - '0') * 10 + (Rx_Buf[3] - '0');
        Joy_Ry = (Rx_Buf[5] - '0') * 10 + (Rx_Buf[6] - '0');
    }

//	printf("Lx:%d	", Joy_Lx);
//	printf("Ly:%d	", Joy_Ly);
//	printf("Rx:%d	", Joy_Rx);
//	printf("Ry:%d	", Joy_Ry);
//	printf("\r\n");

    Map_Lx = Map(Joy_Lx, 10, 90, -90, 90);
    Map_Ly = Map(Joy_Ly, 10, 90, -90, 90);
    Map_Rx = Map(Joy_Rx, 10, 90, -90, 90);
    Map_Ry = Map(Joy_Ry, 10, 90, -90, 90);

//	printf("Map_Lx:%d	", Map_Lx);
//	printf("Map_Ly:%d	", Map_Ly);
//	printf("Map_Rx:%d	", Map_Rx);
//	printf("Map_Ry:%d	", Map_Ry);
//	printf("\r\n");

    car_move(Map_Lx, Map_Ly, Map_Rx);

}

/**************************************************
函数名称：Evadible_Mode(void)
函数功能：避障模式
入口参数：无
返回参数：无
***************************************************/
//void Evadible_Mode(void)
//{
//	float Dis;
//	Dis=Hcsr04GetLength();//超声波模块获取距离
//	forward(200);//前进

//	if(Dis<=15)
//	{
//		backward(200);//后退
//		delay_ms(400);
//		Left_Turn(200);//左转
//		delay_ms(400);
//	}
//}

/**************************************************
函数名称：Follow_Mode(void)
函数功能：跟随模式
入口参数：无
返回参数：无
***************************************************/
//void Follow_Mode(void)
//{
//	float Dis;
//	Dis=Hcsr04GetLength();//超声波模块获取距离
//	if(Dis<=10)
//	{
//		backward(200);//后退
//	}
//	else if(Dis<=30&&Dis>=20)
//	{
//		forward(200);//前进
//	}
//	else Motion_State(ON);
//}
/**************************************************
函数名称：APP_Gravity_Mode(void)
函数功能：APP重力模式
入口参数：无
返回参数：无
***************************************************/
void APP_Gravity_Mode(void)
{
    int i,j=0,Pitch_flag=0;
    int APP_Pitch=0,APP_Roll=0;
    int Pitch_symbel=1,Roll_symbel=1;//偏航角符号
    char Pitch_Buf[10],Roll_Buf[10];
    int Map_pitch, Map_roll;//映射后的偏航角
    int pwm1, pwm2, pwm3, pwm4;
    static int Smoothing_Pitch_Buf[5];//中值滤波数组
    static int Smoothing_Roll_Buf[5];//中值滤波数组
    static int Smoothing_Count=0;//中值滤波采样个数
    int Pitch_temp,Roll_temp;//选择排序变量

//	L_STBY_ON;
//	R_STBY_ON;

    //提取Roll
    for(i=1; i<20; i++)
    {
        if(Pitch_Roll_Buf[i]=='.')break;
        Roll_Buf[i-1]=Pitch_Roll_Buf[i];
    }
    //提取Pitch
    for(i=0; i<20; i++)
    {
        if(Pitch_Roll_Buf[i]==',')
        {
            Pitch_flag=1;
            i++;
        }
        if(Pitch_flag==1)
        {
            if(Pitch_Roll_Buf[i]=='.')
            {
                j=0;
                break;
            }
            Pitch_Buf[j]=Pitch_Roll_Buf[i];
            j++;
        }
    }
    //将Roll字符串转换为整形数据
    j=0;
    for(i=10; i>=0; i--)
    {
        if(Roll_Buf[i]>='0'&&Roll_Buf[i]<='9')
        {
            APP_Roll+=(Roll_Buf[i]-'0')*pow(10,j);
            j++;
        }
        if(Roll_Buf[0]=='-')
        {
            Roll_symbel=-1;
        }
    }
    //将Pitch字符串转换为整形数据
    j=0;
    for(i=10; i>=0; i--)
    {
        if(Pitch_Buf[i]>='0'&&Pitch_Buf[i]<='9')
        {
            APP_Pitch+=(Pitch_Buf[i]-'0')*pow(10,j);
            j++;
        }
        if(Pitch_Buf[0]=='-')
        {
            Pitch_symbel=-1;
        }
    }
    //得到整形偏航角数据
    APP_Pitch=Pitch_symbel*APP_Pitch;
    APP_Roll=Roll_symbel*APP_Roll;
    //采样五次
    Smoothing_Pitch_Buf[Smoothing_Count]=APP_Pitch;
    Smoothing_Roll_Buf[Smoothing_Count]=APP_Roll;
    Smoothing_Count++;
    //选择排序
    if(Smoothing_Count==5)
    {
        Smoothing_Count=0;

        for(j = 0; j < 5 - 1; j++)
        {
            for(i = 0; i < 5 - j; i++)
            {
                if(Smoothing_Pitch_Buf[i] > Smoothing_Pitch_Buf[i + 1])
                {
                    Pitch_temp = Smoothing_Pitch_Buf[i];
                    Smoothing_Pitch_Buf[i] = Smoothing_Pitch_Buf[i + 1];
                    Smoothing_Pitch_Buf[i + 1] = Pitch_temp;
                }
                if(Smoothing_Roll_Buf[i] > Smoothing_Roll_Buf[i + 1])
                {
                    Roll_temp = Smoothing_Roll_Buf[i];
                    Smoothing_Roll_Buf[i] = Smoothing_Roll_Buf[i + 1];
                    Smoothing_Roll_Buf[i + 1] = Roll_temp;
                }
            }
        }
        //中值滤波
        APP_Pitch=Smoothing_Pitch_Buf[2];
        APP_Roll=Smoothing_Roll_Buf[2];

        Map_pitch = Map(APP_Pitch, -90, 90, -127, 127);
        Map_roll = Map(APP_Roll, -90, 90, -127, 127);

        pwm1 =  -Map_pitch + Map_roll;
        pwm2 =  -Map_pitch - Map_roll;
        pwm3 =  -Map_pitch + Map_roll;
        pwm4 =  -Map_pitch - Map_roll;


        pwm1 = Map(pwm1, -127, 127, -499, 499);
        pwm2 = Map(pwm2, -127, 127, -499, 499);
        pwm3 = Map(pwm3, -127, 127, -499, 499);
        pwm4 = Map(pwm4, -127, 127, -499, 499);



        if (pwm1 < 20 && pwm1 >-20)pwm1 = 0;
        if (pwm2 < 20 && pwm2 >-20)pwm2 = 0;
        if (pwm3 < 20 && pwm3 >-20)pwm3 = 0;
        if (pwm4 < 20 && pwm4 >-20)pwm4 = 0;

        if (pwm1 > 499)pwm1 = 499;
        if (pwm2 > 499)pwm2 = 499;
        if (pwm3 > 499)pwm3 = 499;
        if (pwm4 > 499)pwm4 = 499;

        if (pwm1 < -499)pwm1 = -499;
        if (pwm2 < -499)pwm2 = -499;
        if (pwm3 < -499)pwm3 = -499;
        if (pwm4 < -499)pwm4 = -499;



        if(pwm1>=0)
        {
//			TIM_SetCompare4(TIM2,500-pwm1);//L_BIN2:左上轮
//			L_BIN2_ON;

        }
        else if(pwm1<0)
        {
//			pwm1=abs(pwm1);
//			TIM_SetCompare4(TIM2,pwm1);//L_BIN2:左上轮
//			L_BIN2_OFF;
        }

        if(pwm2>=0)
        {
//			TIM_SetCompare3(TIM2,pwm2);//L_AIN2:右上轮
//			L_AIN2_OFF;
        }
        else if(pwm2<0)
        {
//			pwm2=abs(pwm2);
//			TIM_SetCompare3(TIM2,500-pwm2);//L_AIN2:右上轮
//			L_AIN2_ON;
        }

        if(pwm3>=0)
        {
//			TIM_SetCompare1(TIM2,500-pwm3);//R_AIN2:右下轮
//			R_AIN2_ON;
        }
        else if(pwm3<0)
        {
//			pwm3=abs(pwm3);
//			TIM_SetCompare1(TIM2,pwm3);//R_AIN2:右下轮
//			R_AIN2_OFF;
        }

        if(pwm4>=0)
        {
//			TIM_SetCompare2(TIM2,pwm4);//R_BIN2:左下轮
//			R_BIN2_OFF;
        }
        else if(pwm4<0)
        {
//			pwm4=abs(pwm4);
//			TIM_SetCompare2(TIM2,500-pwm4);//R_BIN2:左下轮
//			R_BIN2_ON;
        }

        memset(Smoothing_Pitch_Buf,0,sizeof(Smoothing_Pitch_Buf));
        memset(Smoothing_Roll_Buf,0,sizeof(Smoothing_Roll_Buf));
        delay_ms(1);
    }

    memset(Roll_Buf,0,10);
    memset(Pitch_Buf,0,10);

    delay_ms(1);
}

/**************************************************
函数名称：void Control(void)
函数功能：麦轮小车控制函数
入口参数：无
返回参数：无
***************************************************/
void Control(void)
{
    Bluetooth_Mode();
}

