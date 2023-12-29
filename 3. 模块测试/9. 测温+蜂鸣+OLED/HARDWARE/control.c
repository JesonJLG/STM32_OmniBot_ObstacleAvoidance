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
extern char Lx_Buf[10];			//��ҡ�����ݽ��ջ�����
extern char Rx_Buf[10];			//��ҡ�����ݽ��ջ�����
extern char Pitch_Roll_Buf[20];	//APPƫ�������ݽ��ջ�����
u8 Eva_RxBuf[4];//����ָֹͣ������

float dist_middle;
float dist_left;
float dist_right;

/**************************************************
�������ƣ�Map(int val,int in_min,int in_max,int out_min,int out_max)
�������ܣ�ӳ�亯��
��ڲ�����	val:��Ҫӳ�����ֵ
            in_min:����ֵ�ķ�Χ��Сֵ
            in_max:����ֵ�ķ�Χ���ֵ
            out_min:���ֵ�ķ�Χ��Сֵ
            out_max:���ֵ�ķ�Χ���ֵ

���ز�����ӳ������ֵ
***************************************************/
int Map(int val, int in_min, int in_max, int out_min, int out_max)
{
    return (int)(val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**************************************************
�������ƣ�Bluetooth_Mode(void)
�������ܣ�����ң��ģʽ
��ڲ�������
���ز�������
***************************************************/
void Bluetooth_Mode(void)
{
    if (mode_flag == 1)		    APP_Joy_Mode();		//APPҡ��ģʽ
    else if (mode_flag == 2)	APP_Gravity_Mode();	//APP����ģʽ
    else if (mode_flag == 3)	Evadible_Mode();	//С������ģʽ
//    else if(mode_flag==4)		Follow_Mode();		//С������ģʽ
    else
    {
        printf("mode_flag:%d\r\n", mode_flag);
    }
}

/**************************************************
�������ƣ�Evadible_Mode(void)
�������ܣ�����ģʽ
��ڲ�������
���ز�������
***************************************************/
void Evadible_Mode(void)
{
    // float Dis;
    // static char tmpflag = 0;
    // Dis = Hcsr04GetLength();//������ģ���ȡ����
    // Car_Forward(350);//ǰ��

    // if (Dis <= 15)
    // {
    //     tmpflag = 1;
    //     Car_Backward(350);//����
    //     delay_ms(500);
    //     Car_TurnLeft(350);//��ת
    //     delay_ms(500);
    // }
    // else
    // {
    //     if (tmpflag == 1)
    //     {
    //         if (Dis > 150)
    //         {
    //             Car_Backward(350);//����
    //             delay_ms(500);
    //             Car_TurnLeft(350);//��ת
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
    if (dist_middle > 35) { //����������35cm�Ϳ���ǰ��
        Car_Forward(350);//ǰ��
    }
    else if ((dist_middle > 10) && (dist_middle < 35)) { //����С��35cm������10cmʱ
        Car_Stop();//��ͣ��
        Servo_SetAngle(0);      //����ഫ����ͨ��SG90ת�����
        dist_left = Hcsr04GetLength();
        delay_ms(300);//����ʵ�����

        Servo_SetAngle(180);    //����ഫ����ͨ��SG90ת���ұ�
        dist_right = Hcsr04GetLength();
        delay_ms(300);//����ʵ�����

        if (dist_right > dist_left) { //�ı߾�������ı�ת
            Car_TurnRight(350);
            delay_ms(300);//����ʵ�����
            Car_Stop();
        }
        else {
            Car_TurnLeft(350);
            delay_ms(300);//����ʵ�����
            Car_Stop();
        }
    }
    else { //�������Ѿ�С��10cmʱ
        Car_Backward(350);//��ʱ�����Ѿ�Σ���ˣ���Ҫ����һ��
        delay_ms(300);//����ʵ�����
        Car_Stop();
    }

}


/**************************************************
�������ƣ�Follow_Mode(void)
�������ܣ�����ģʽ
��ڲ�������
���ز�������
***************************************************/
void Follow_Mode(void)
{
    float Dis;
    Dis = Hcsr04GetLength();//������ģ���ȡ����
    if (Dis <= 10)
    {
        Car_Backward(400);//����
    }
    else if (Dis <= 30 && Dis >= 20)
    {
        Car_Forward(400);//ǰ��
    }
}

/**************************************************
�������ƣ�APP_Joy_Mode(void)
�������ܣ�����ģʽ��ҡ��ң��
��ڲ�������
���ز�������
***************************************************/
void APP_Joy_Mode(void)
{
    int Joy_Lx = 50, Joy_Ly = 50, Joy_Rx = 50, Joy_Ry = 50;	//middle
    int Map_Lx, Map_Ly, Map_Rx, Map_Ry;						//ӳ��ֵ
    int pwm1, pwm2, pwm3, pwm4;								//�ٶȣ�

    /*------------����ҡ������-------------*/
    if (Lx_Buf[0] == 'L')
    {   //"LX50Y50"
        Joy_Lx = (Lx_Buf[2] - '0') * 10 + (Lx_Buf[3] - '0');	//ACSIIתint
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
    /*------------ҡ��ӳ������ת-------------*/
    Map_Lx = Map(Joy_Lx, 10, 90, -127, 127);
    Map_Ly = Map(Joy_Ly, 10, 90, -127, 127);
    Map_Rx = Map(Joy_Rx, 10, 90, -127, 127);
    Map_Ry = Map(Joy_Ry, 10, 90, -127, 127);
    /*
        ����С��������ϵΪX������Ϊ����Y������Ϊ����yaw��˳ʱ��Ϊ������ҡ�������������������
        Motor Mapping:
          M1   M2
            CAR
          M4   M3
    */
    /*------------�������˶�ѧ����OK-------------*/
    pwm1 = -Map_Ly + Map_Lx - Map_Ry + Map_Rx;	//MotorB
    pwm2 = -Map_Ly - Map_Lx - Map_Ry - Map_Rx;	//MotorC
    pwm3 = -Map_Ly + Map_Lx - Map_Ry - Map_Rx;	//MotorD
    pwm4 = -Map_Ly - Map_Lx - Map_Ry + Map_Rx;	//MotorA

    /*------------ӳ����PWMռ�ձȷ�Χ-------------*/
    pwm1 = Map(pwm1, -127, 127, -1000, 1000);
    pwm2 = Map(pwm2, -127, 127, -1000, 1000);
    pwm3 = Map(pwm3, -127, 127, -1000, 1000);
    pwm4 = Map(pwm4, -127, 127, -1000, 1000);

    /*------------���˵�ռ�ձȣ���Ϊ�����������-------------*/
    if (pwm1 < 200 && pwm1 >-200)	pwm1 = 0;
    if (pwm2 < 200 && pwm2 >-200)	pwm2 = 0;
    if (pwm3 < 200 && pwm3 >-200)	pwm3 = 0;
    if (pwm4 < 200 && pwm4 >-200)	pwm4 = 0;

    /*------------�����������ռ�ձ�-------------*/
    if (pwm1 > 1000)	pwm1 = 1000;
    if (pwm2 > 1000)	pwm2 = 1000;
    if (pwm3 > 1000)	pwm3 = 1000;
    if (pwm4 > 1000)	pwm4 = 1000;

    /*------------���Ʒ������ռ�ձ�-------------*/
    if (pwm1 < -1000)	pwm1 = -1000;
    if (pwm2 < -1000)	pwm2 = -1000;
    if (pwm3 < -1000)	pwm3 = -1000;
    if (pwm4 < -1000)	pwm4 = -1000;

    printf("pwm1:%d	", pwm1);
    printf("pwm2:%d	", pwm1);
    printf("pwm3:%d	", pwm1);
    printf("pwm4:%d	", pwm1);
    printf("\r\n");

    /*------------������B-------------*/
    if (pwm1 >= 0)		//B����ת
    {
        //		TIM_SetCompare4(TIM2,500-pwm1);//L_BIN2:������
        //		L_BIN2_ON;
        MotorB_SetSpeed(1, pwm1);
    }
    else if (pwm1 < 0)	//B�ַ�ת
    {
        //		pwm1=abs(pwm1);
        //		TIM_SetCompare4(TIM2,pwm1);//L_BIN2:������
        //		L_BIN2_OFF;
        MotorB_SetSpeed(0, -pwm1);
    }

    /*------------������C-------------*/
    if (pwm2 >= 0)
    {
        //		TIM_SetCompare3(TIM2,pwm2);//L_AIN2:������
        //		L_AIN2_OFF;
        MotorC_SetSpeed(1, pwm2);
    }
    else if (pwm2 < 0)
    {
        //		pwm2=abs(pwm2);
        //		TIM_SetCompare3(TIM2,500-pwm2);//L_AIN2:������
        //		L_AIN2_ON;
        MotorC_SetSpeed(0, -pwm2);
    }

    /*------------������D-------------*/
    if (pwm3 >= 0)
    {
        //		TIM_SetCompare1(TIM2,500-pwm3);//R_AIN2:������
        //		R_AIN2_ON;
        MotorD_SetSpeed(1, pwm3);
    }
    else if (pwm3 < 0)
    {
        //		pwm3=abs(pwm3);
        //		TIM_SetCompare1(TIM2,pwm3);//R_AIN2:������
        //		R_AIN2_OFF;
        MotorD_SetSpeed(0, -pwm3);
    }

    /*------------������A-------------*/
    if (pwm4 >= 0)
    {
        //		TIM_SetCompare2(TIM2,pwm4);//R_BIN2:������
        //		R_BIN2_OFF;
        MotorA_SetSpeed(1, pwm4);
    }
    else if (pwm4 < 0)
    {
        //		pwm4=abs(pwm4);
        //		TIM_SetCompare2(TIM2,500-pwm4);//R_BIN2:������
        //		R_BIN2_ON;
        MotorA_SetSpeed(0, -pwm4);
    }
    delay_ms(10);
    //printf(Lx_Buf);
    //printf(Rx_Buf);
    //printf("\n");
}

/**************************************************
�������ƣ�APP_Gravity_Mode(void)
�������ܣ�APP����ģʽ
��ڲ�������
���ز�������
***************************************************/
void APP_Gravity_Mode(void)
{
    int i, j = 0, Pitch_flag = 0;
    int APP_Pitch = 0, APP_Roll = 0;
    int Pitch_symbel = 1, Roll_symbel = 1;//ƫ���Ƿ���
    char Pitch_Buf[10], Roll_Buf[10];
    int Map_pitch, Map_roll;//ӳ����ƫ����
    int pwm1, pwm2, pwm3, pwm4;
    static int Smoothing_Pitch_Buf[5];//��ֵ�˲�����
    static int Smoothing_Roll_Buf[5];//��ֵ�˲�����
    static int Smoothing_Count = 0;//��ֵ�˲���������
    int Pitch_temp, Roll_temp;//ѡ���������

//	L_STBY_ON;
//	R_STBY_ON;

    //��ȡRoll
    for (i = 1; i < 20; i++)
    {
        if (Pitch_Roll_Buf[i] == '.')break;
        Roll_Buf[i - 1] = Pitch_Roll_Buf[i];
    }
    //��ȡPitch
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
    //��Roll�ַ���ת��Ϊ��������
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
    //��Pitch�ַ���ת��Ϊ��������
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
    //�õ�����ƫ��������
    APP_Pitch = Pitch_symbel * APP_Pitch;
    APP_Roll = Roll_symbel * APP_Roll;
    //�������
    Smoothing_Pitch_Buf[Smoothing_Count] = APP_Pitch;
    Smoothing_Roll_Buf[Smoothing_Count] = APP_Roll;
    Smoothing_Count++;
    //ѡ������
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
        //��ֵ�˲�
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
            //			TIM_SetCompare4(TIM2,500-pwm1);//L_BIN2:������
            //			L_BIN2_ON;

        }
        else if (pwm1 < 0)
        {
            //			pwm1=abs(pwm1);
            //			TIM_SetCompare4(TIM2,pwm1);//L_BIN2:������
            //			L_BIN2_OFF;
        }

        if (pwm2 >= 0)
        {
            //			TIM_SetCompare3(TIM2,pwm2);//L_AIN2:������
            //			L_AIN2_OFF;
        }
        else if (pwm2 < 0)
        {
            //			pwm2=abs(pwm2);
            //			TIM_SetCompare3(TIM2,500-pwm2);//L_AIN2:������
            //			L_AIN2_ON;
        }

        if (pwm3 >= 0)
        {
            //			TIM_SetCompare1(TIM2,500-pwm3);//R_AIN2:������
            //			R_AIN2_ON;
        }
        else if (pwm3 < 0)
        {
            //			pwm3=abs(pwm3);
            //			TIM_SetCompare1(TIM2,pwm3);//R_AIN2:������
            //			R_AIN2_OFF;
        }

        if (pwm4 >= 0)
        {
            //			TIM_SetCompare2(TIM2,pwm4);//R_BIN2:������
            //			R_BIN2_OFF;
        }
        else if (pwm4 < 0)
        {
            //			pwm4=abs(pwm4);
            //			TIM_SetCompare2(TIM2,500-pwm4);//R_BIN2:������
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
�������ƣ�void Control(void)
�������ܣ�����С�����ƺ���
��ڲ�������
���ز�������
***************************************************/
void Control(void)
{
    Bluetooth_Mode();
}

