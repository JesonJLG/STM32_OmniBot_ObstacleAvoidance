#include "control.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
#include "math.h"
#include "string.h"
#include "hcsr.h"
#include "beep.h"
#include "servo.h"

extern u8 mode_flag;
extern char Lx_Buf[10];			//左摇杆数据接收缓冲区
extern char Rx_Buf[10];			//右摇杆数据接收缓冲区
extern char Pitch_Roll_Buf[20];	//APP偏航角数据接收缓冲区
u8 Eva_RxBuf[4];//避障停止指令数组

float dist_middle;
float dist_left;
float dist_right;

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
int Map(int val, int in_min, int in_max, int out_min, int out_max)
{
    return (int)(val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**************************************************
函数名称：Bluetooth_Mode(void)
函数功能：蓝牙遥控模式
入口参数：无
返回参数：无
***************************************************/
void Bluetooth_Mode(void)
{
    if (mode_flag == 1)		    APP_Joy_Mode();		//APP摇杆模式
    else if (mode_flag == 2)	APP_Gravity_Mode();	//APP重力模式
    else if (mode_flag == 3)	Evadible_Mode();	//小车避障模式
//    else if(mode_flag==4)		Follow_Mode();		//小车跟随模式
    else
    {
        printf("mode_flag:%d\r\n", mode_flag);
    }
}

/**************************************************
函数名称：Evadible_Mode(void)
函数功能：避障模式
入口参数：无
返回参数：无
***************************************************/
void Evadible_Mode(void)
{
    // float Dis;
    // static char tmpflag = 0;
    // Dis = Hcsr04GetLength();//超声波模块获取距离
    // Car_Forward(350);//前进

    // if (Dis <= 15)
    // {
    //     tmpflag = 1;
    //     Car_Backward(350);//后退
    //     delay_ms(500);
    //     Car_TurnLeft(350);//左转
    //     delay_ms(500);
    // }
    // else
    // {
    //     if (tmpflag == 1)
    //     {
    //         if (Dis > 150)
    //         {
    //             Car_Backward(350);//后退
    //             delay_ms(500);
    //             Car_TurnLeft(350);//左转
    //             delay_ms(500);
    //         }
    //     }
    //     tmpflag = 0;
    // }

    deal_dist();

}

static void deal_dist(void)
{
    dist_middle = Hcsr04GetLength();
    if (dist_middle > 35) { //如果距离大于35cm就可以前进
        Car_Forward(350);//前进
    }
    else if ((dist_middle > 10) && (dist_middle < 35)) { //距离小于35cm但大于10cm时
        Car_Stop();//先停下
        Servo_SetAngle(0);      //将测距传感器通过SG90转到左边
        dist_left = Hcsr04GetLength();
        delay_ms(300);//根据实际情况

        Servo_SetAngle(180);    //将测距传感器通过SG90转到右边
        dist_right = Hcsr04GetLength();
        delay_ms(300);//根据实际情况

        if (dist_right > dist_left) { //哪边距离大往哪边转
            Car_TurnRight(350);
            delay_ms(300);//根据实际情况
            Car_Stop();
        }
        else {
            Car_TurnLeft(350);
            delay_ms(300);//根据实际情况
            Car_Stop();
        }
    }
    else { //当距离已经小于10cm时
        Car_Backward(350);//此时距离已经危险了，需要后退一下
        delay_ms(300);//根据实际情况
        Car_Stop();
    }

}


/**************************************************
函数名称：Follow_Mode(void)
函数功能：跟随模式
入口参数：无
返回参数：无
***************************************************/
void Follow_Mode(void)
{
    float Dis;
    Dis = Hcsr04GetLength();//超声波模块获取距离
    if (Dis <= 10)
    {
        Car_Backward(400);//后退
    }
    else if (Dis <= 30 && Dis >= 20)
    {
        Car_Forward(400);//前进
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
    int Joy_Lx = 50, Joy_Ly = 50, Joy_Rx = 50, Joy_Ry = 50;	//middle
    int Map_Lx, Map_Ly, Map_Rx, Map_Ry;						//映射值
    int pwm1, pwm2, pwm3, pwm4;								//速度？

    /*------------解析摇杆数据-------------*/
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
    /*------------摇杆映射正反转-------------*/
    Map_Lx = Map(Joy_Lx, 10, 90, -127, 127);
    Map_Ly = Map(Joy_Ly, 10, 90, -127, 127);
    Map_Rx = Map(Joy_Rx, 10, 90, -127, 127);
    Map_Ry = Map(Joy_Ry, 10, 90, -127, 127);
    /*
        对于小车，坐标系为X轴向右为正，Y轴向下为正，yaw轴顺时针为正（由摇杆数据增长方向决定）
        Motor Mapping:
          M1   M2
            CAR
          M4   M3
    */
    /*------------麦轮逆运动学解算OK-------------*/
    pwm1 = -Map_Ly + Map_Lx - Map_Ry + Map_Rx;	//MotorB
    pwm2 = -Map_Ly - Map_Lx - Map_Ry - Map_Rx;	//MotorC
    pwm3 = -Map_Ly + Map_Lx - Map_Ry - Map_Rx;	//MotorD
    pwm4 = -Map_Ly - Map_Lx - Map_Ry + Map_Rx;	//MotorA

    /*------------映射至PWM占空比范围-------------*/
    pwm1 = Map(pwm1, -127, 127, -1000, 1000);
    pwm2 = Map(pwm2, -127, 127, -1000, 1000);
    pwm3 = Map(pwm3, -127, 127, -1000, 1000);
    pwm4 = Map(pwm4, -127, 127, -1000, 1000);

    /*------------过滤低占空比（因为带不动电机）-------------*/
    if (pwm1 < 200 && pwm1 >-200)	pwm1 = 0;
    if (pwm2 < 200 && pwm2 >-200)	pwm2 = 0;
    if (pwm3 < 200 && pwm3 >-200)	pwm3 = 0;
    if (pwm4 < 200 && pwm4 >-200)	pwm4 = 0;

    /*------------限制正向最大占空比-------------*/
    if (pwm1 > 1000)	pwm1 = 1000;
    if (pwm2 > 1000)	pwm2 = 1000;
    if (pwm3 > 1000)	pwm3 = 1000;
    if (pwm4 > 1000)	pwm4 = 1000;

    /*------------限制反向最大占空比-------------*/
    if (pwm1 < -1000)	pwm1 = -1000;
    if (pwm2 < -1000)	pwm2 = -1000;
    if (pwm3 < -1000)	pwm3 = -1000;
    if (pwm4 < -1000)	pwm4 = -1000;

    printf("pwm1:%d	", pwm1);
    printf("pwm2:%d	", pwm1);
    printf("pwm3:%d	", pwm1);
    printf("pwm4:%d	", pwm1);
    printf("\r\n");

    /*------------左上轮B-------------*/
    if (pwm1 >= 0)		//B轮正转
    {
        //		TIM_SetCompare4(TIM2,500-pwm1);//L_BIN2:左上轮
        //		L_BIN2_ON;
        MotorB_SetSpeed(1, pwm1);
    }
    else if (pwm1 < 0)	//B轮反转
    {
        //		pwm1=abs(pwm1);
        //		TIM_SetCompare4(TIM2,pwm1);//L_BIN2:左上轮
        //		L_BIN2_OFF;
        MotorB_SetSpeed(0, -pwm1);
    }

    /*------------右上轮C-------------*/
    if (pwm2 >= 0)
    {
        //		TIM_SetCompare3(TIM2,pwm2);//L_AIN2:右上轮
        //		L_AIN2_OFF;
        MotorC_SetSpeed(1, pwm2);
    }
    else if (pwm2 < 0)
    {
        //		pwm2=abs(pwm2);
        //		TIM_SetCompare3(TIM2,500-pwm2);//L_AIN2:右上轮
        //		L_AIN2_ON;
        MotorC_SetSpeed(0, -pwm2);
    }

    /*------------右下轮D-------------*/
    if (pwm3 >= 0)
    {
        //		TIM_SetCompare1(TIM2,500-pwm3);//R_AIN2:右下轮
        //		R_AIN2_ON;
        MotorD_SetSpeed(1, pwm3);
    }
    else if (pwm3 < 0)
    {
        //		pwm3=abs(pwm3);
        //		TIM_SetCompare1(TIM2,pwm3);//R_AIN2:右下轮
        //		R_AIN2_OFF;
        MotorD_SetSpeed(0, -pwm3);
    }

    /*------------左下轮A-------------*/
    if (pwm4 >= 0)
    {
        //		TIM_SetCompare2(TIM2,pwm4);//R_BIN2:左下轮
        //		R_BIN2_OFF;
        MotorA_SetSpeed(1, pwm4);
    }
    else if (pwm4 < 0)
    {
        //		pwm4=abs(pwm4);
        //		TIM_SetCompare2(TIM2,500-pwm4);//R_BIN2:左下轮
        //		R_BIN2_ON;
        MotorA_SetSpeed(0, -pwm4);
    }
    delay_ms(10);
    //printf(Lx_Buf);
    //printf(Rx_Buf);
    //printf("\n");
}

/**************************************************
函数名称：APP_Gravity_Mode(void)
函数功能：APP重力模式
入口参数：无
返回参数：无
***************************************************/
void APP_Gravity_Mode(void)
{
    int i, j = 0, Pitch_flag = 0;
    int APP_Pitch = 0, APP_Roll = 0;
    int Pitch_symbel = 1, Roll_symbel = 1;//偏航角符号
    char Pitch_Buf[10], Roll_Buf[10];
    int Map_pitch, Map_roll;//映射后的偏航角
    int pwm1, pwm2, pwm3, pwm4;
    static int Smoothing_Pitch_Buf[5];//中值滤波数组
    static int Smoothing_Roll_Buf[5];//中值滤波数组
    static int Smoothing_Count = 0;//中值滤波采样个数
    int Pitch_temp, Roll_temp;//选择排序变量

//	L_STBY_ON;
//	R_STBY_ON;

    //提取Roll
    for (i = 1; i < 20; i++)
    {
        if (Pitch_Roll_Buf[i] == '.')break;
        Roll_Buf[i - 1] = Pitch_Roll_Buf[i];
    }
    //提取Pitch
    for (i = 0; i < 20; i++)
    {
        if (Pitch_Roll_Buf[i] == ',')
        {
            Pitch_flag = 1;
            i++;
        }
        if (Pitch_flag == 1)
        {
            if (Pitch_Roll_Buf[i] == '.')
            {
                j = 0;
                break;
            }
            Pitch_Buf[j] = Pitch_Roll_Buf[i];
            j++;
        }
    }
    //将Roll字符串转换为整形数据
    j = 0;
    for (i = 10; i >= 0; i--)
    {
        if (Roll_Buf[i] >= '0' && Roll_Buf[i] <= '9')
        {
            APP_Roll += (Roll_Buf[i] - '0') * pow(10, j);
            j++;
        }
        if (Roll_Buf[0] == '-')
        {
            Roll_symbel = -1;
        }
    }
    //将Pitch字符串转换为整形数据
    j = 0;
    for (i = 10; i >= 0; i--)
    {
        if (Pitch_Buf[i] >= '0' && Pitch_Buf[i] <= '9')
        {
            APP_Pitch += (Pitch_Buf[i] - '0') * pow(10, j);
            j++;
        }
        if (Pitch_Buf[0] == '-')
        {
            Pitch_symbel = -1;
        }
    }
    //得到整形偏航角数据
    APP_Pitch = Pitch_symbel * APP_Pitch;
    APP_Roll = Roll_symbel * APP_Roll;
    //采样五次
    Smoothing_Pitch_Buf[Smoothing_Count] = APP_Pitch;
    Smoothing_Roll_Buf[Smoothing_Count] = APP_Roll;
    Smoothing_Count++;
    //选择排序
    if (Smoothing_Count == 5)
    {
        Smoothing_Count = 0;

        for (j = 0; j < 5 - 1; j++)
        {
            for (i = 0; i < 5 - j; i++)
            {
                if (Smoothing_Pitch_Buf[i] > Smoothing_Pitch_Buf[i + 1])
                {
                    Pitch_temp = Smoothing_Pitch_Buf[i];
                    Smoothing_Pitch_Buf[i] = Smoothing_Pitch_Buf[i + 1];
                    Smoothing_Pitch_Buf[i + 1] = Pitch_temp;
                }
                if (Smoothing_Roll_Buf[i] > Smoothing_Roll_Buf[i + 1])
                {
                    Roll_temp = Smoothing_Roll_Buf[i];
                    Smoothing_Roll_Buf[i] = Smoothing_Roll_Buf[i + 1];
                    Smoothing_Roll_Buf[i + 1] = Roll_temp;
                }
            }
        }
        //中值滤波
        APP_Pitch = Smoothing_Pitch_Buf[2];
        APP_Roll = Smoothing_Roll_Buf[2];

        Map_pitch = Map(APP_Pitch, -90, 90, -127, 127);
        Map_roll = Map(APP_Roll, -90, 90, -127, 127);

        pwm1 = -Map_pitch + Map_roll;
        pwm2 = -Map_pitch - Map_roll;
        pwm3 = -Map_pitch + Map_roll;
        pwm4 = -Map_pitch - Map_roll;


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



        if (pwm1 >= 0)
        {
            //			TIM_SetCompare4(TIM2,500-pwm1);//L_BIN2:左上轮
            //			L_BIN2_ON;

        }
        else if (pwm1 < 0)
        {
            //			pwm1=abs(pwm1);
            //			TIM_SetCompare4(TIM2,pwm1);//L_BIN2:左上轮
            //			L_BIN2_OFF;
        }

        if (pwm2 >= 0)
        {
            //			TIM_SetCompare3(TIM2,pwm2);//L_AIN2:右上轮
            //			L_AIN2_OFF;
        }
        else if (pwm2 < 0)
        {
            //			pwm2=abs(pwm2);
            //			TIM_SetCompare3(TIM2,500-pwm2);//L_AIN2:右上轮
            //			L_AIN2_ON;
        }

        if (pwm3 >= 0)
        {
            //			TIM_SetCompare1(TIM2,500-pwm3);//R_AIN2:右下轮
            //			R_AIN2_ON;
        }
        else if (pwm3 < 0)
        {
            //			pwm3=abs(pwm3);
            //			TIM_SetCompare1(TIM2,pwm3);//R_AIN2:右下轮
            //			R_AIN2_OFF;
        }

        if (pwm4 >= 0)
        {
            //			TIM_SetCompare2(TIM2,pwm4);//R_BIN2:左下轮
            //			R_BIN2_OFF;
        }
        else if (pwm4 < 0)
        {
            //			pwm4=abs(pwm4);
            //			TIM_SetCompare2(TIM2,500-pwm4);//R_BIN2:左下轮
            //			R_BIN2_ON;
        }

        memset(Smoothing_Pitch_Buf, 0, sizeof(Smoothing_Pitch_Buf));
        memset(Smoothing_Roll_Buf, 0, sizeof(Smoothing_Roll_Buf));
        delay_ms(1);
    }

    memset(Roll_Buf, 0, 10);
    memset(Pitch_Buf, 0, 10);

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

