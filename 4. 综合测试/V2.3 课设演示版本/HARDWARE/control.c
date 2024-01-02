#include "control.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
#include "math.h"
#include "string.h"
#include "hcsr.h"
#include "beep.h"
#include "servo.h"
#include "serial.h"

extern u8 mode_flag;
extern char Lr_RxPacket[10];			//��ҡ�����ݽ��ջ�����
extern char Rr_RxPacket[10];			//��ҡ�����ݽ��ջ�����
extern char Pitch_Roll_RxPacket[20];	//APPƫ�������ݽ��ջ�����
extern char Pwm_RxPacket[10];			//"P[...]\r\n"	Pwm_RxPacket[0]Ϊpwm_limit	APP�ٶȻ���

u8 Eva_RxBuf[4];//����ָֹͣ������
float dist_middle;
float dist_left;
float dist_right;
int pwm_limit = 999;	//�ɵ�ռ�ձ�����

extern char servo_dir;

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
�������ƣ�void Control(void)
�������ܣ�����С�����ƺ���
��ڲ�������
���ز�������
***************************************************/
void Control(void)
{
    Bluetooth_Mode();
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
    else if(mode_flag==4)		Follow_Mode();		//С������ģʽ
    else
    {
//        printf("mode_flag:%d\r\n", mode_flag);
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
    int pwm1, pwm2, pwm3, pwm4;								//ռ�ձ��ٶ�

    /*------------����ҡ������-------------*/
//    printf("Pwm_RxPacket[0]:%c\t", Pwm_RxPacket[0]);
//    printf("Pwm_RxPacket[1]:%c\t", Pwm_RxPacket[1]);
//    printf("Pwm_RxPacket[2]:%c\t", Pwm_RxPacket[2]);
//    printf("Pwm_RxPacket[3]:%c\t", Pwm_RxPacket[3]);
//    printf("Pwm_RxPacket[4]:%c\t", Pwm_RxPacket[4]);
//    printf("Pwm_RxPacket[5]:%c\t", Pwm_RxPacket[5]);
//    printf("\r\n");
	sgReset();
	
    if (Pwm_RxPacket[0] == 'P')
    {   //"P999*"
        pwm_limit = (Pwm_RxPacket[1]-'0')*100 + (Pwm_RxPacket[2]-'0')*10 + (Pwm_RxPacket[3]-'0'); 	//ע�����pwm_limit
        printf("pwm_limit:%d\t", pwm_limit);
        Serial_Printf("*G%d*", pwm_limit);
    }
    if (Lr_RxPacket[0] == 'L')
    {   //"LX50Y50*"
        Joy_Lx = (Lr_RxPacket[2] - '0') * 10 + (Lr_RxPacket[3] - '0');	//ACSIIתint
        Joy_Ly = (Lr_RxPacket[5] - '0') * 10 + (Lr_RxPacket[6] - '0');
    }
    if (Rr_RxPacket[0] == 'R')
    {   //"RX50Y50*"
        Joy_Rx = (Rr_RxPacket[2] - '0') * 10 + (Rr_RxPacket[3] - '0');
        Joy_Ry = (Rr_RxPacket[5] - '0') * 10 + (Rr_RxPacket[6] - '0');
    }

//    printf("Joy_Lx:%d\t", Joy_Lx);
//    printf("Joy_Ly:%d\t", Joy_Ly);
//    printf("Joy_Rx:%d\t", Joy_Rx);
//    printf("Joy_Ry:%d\t", Joy_Ry);
//    printf("\r\n");
    /*------------ҡ��ӳ������ת-------------*/
    Map_Lx = Map(Joy_Lx, 10, 90, -127, 127);
    Map_Ly = Map(Joy_Ly, 10, 90, -127, 127);
    Map_Rx = Map(Joy_Rx, 10, 90, -127, 127);
    Map_Ry = Map(Joy_Ry, 10, 90, -127, 127);
    /*
        ����С��������ϵΪX������Ϊ����Y������Ϊ����yaw��˳ʱ��Ϊ������ҡ�������������������
        Motor Mapping:
          MB   MC
            CAR
          MA   MD
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
    if (pwm1 > pwm_limit)	pwm1 = pwm_limit;
    if (pwm2 > pwm_limit)	pwm2 = pwm_limit;
    if (pwm3 > pwm_limit)	pwm3 = pwm_limit;
    if (pwm4 > pwm_limit)	pwm4 = pwm_limit;

    /*------------���Ʒ������ռ�ձ�-------------*/
    if (pwm1 < -pwm_limit)	pwm1 = -pwm_limit;
    if (pwm2 < -pwm_limit)	pwm2 = -pwm_limit;
    if (pwm3 < -pwm_limit)	pwm3 = -pwm_limit;
    if (pwm4 < -pwm_limit)	pwm4 = -pwm_limit;

    printf("pwm1:%d  \t", pwm1);
    printf("pwm2:%d  \t", pwm1);
    printf("pwm3:%d  \t", pwm1);
    printf("pwm4:%d  \t", pwm1);
    printf("\r\n");

    /*------------������B-------------*/
    if (pwm1 >= 0)		//B����ת
    {
        MotorB_SetSpeed(1, pwm1);
    }
    else if (pwm1 < 0)	//B�ַ�ת
    {
        MotorB_SetSpeed(0, -pwm1);
    }

    /*------------������C-------------*/
    if (pwm2 >= 0)
    {
        MotorC_SetSpeed(1, pwm2);
    }
    else if (pwm2 < 0)
    {
        MotorC_SetSpeed(0, -pwm2);
    }

    /*------------������D-------------*/
    if (pwm3 >= 0)
    {
        MotorD_SetSpeed(1, pwm3);
    }
    else if (pwm3 < 0)
    {
        MotorD_SetSpeed(0, -pwm3);
    }

    /*------------������A-------------*/
    if (pwm4 >= 0)
    {
        MotorA_SetSpeed(1, pwm4);
    }
    else if (pwm4 < 0)
    {
        MotorA_SetSpeed(0, -pwm4);
    }
    delay_ms(10);
}

/**************************************************
�������ƣ�Evadible_Mode(void)
�������ܣ�����ģʽ
��ڲ�������
���ز�������
***************************************************/
void Evadible_Mode(void)
{
    deal_dist2();
}

/*------------ҡͷ����-------------*/
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
            delay_ms(500);//����ʵ�����
            Car_Stop();
        }
        else {
            Car_TurnLeft(350);
            delay_ms(500);//����ʵ�����
            Car_Stop();
        }
    }
    else { //�������Ѿ�С��10cmʱ
        Car_Backward(350);//��ʱ�����Ѿ�Σ���ˣ���Ҫ����һ��
        delay_ms(300);//����ʵ�����
        Car_Stop();
    }

}

/*------------ҡͷ����2-------------*/
static void deal_dist2(void)
{
    static float disMiddle, disLeft, disRight;
    sgReset();

    disMiddle = Hcsr04GetLength();
    if(disMiddle > 35)
    {
		Car_Forward(400);//ǰ��
		delay_ms(400);
    }
    else if(disMiddle < 15)
    {
        Car_Backward(400);
        delay_ms(400);
    }
    else
    {
        Car_Stop();		//ֹͣ
        /*------------�����-------------*/
        sgLeft();
        servo_dir = LIFT;
        delay_ms(300);
        disLeft = Hcsr04GetLength();
        /*------------���ұ�-------------*/
        sgRight();
        servo_dir = RIGHT;
        delay_ms(300);
        disRight = Hcsr04GetLength();
        /*------------����-------------*/
        if(disLeft < disRight)
        {
            Car_TurnRight(400);
            delay_ms(1000);
            Car_Stop();
        }
        if(disRight < disLeft)
        {
            Car_TurnLeft(400);
            delay_ms(1000);
            Car_Stop();
        }
    }
    delay_ms(50);
}

/*------------�򵥱���-------------*/
static void deal_dis(void)
{
    float Dis;
    static char tmpflag = 0;
    Dis = Hcsr04GetLength();//������ģ���ȡ����
    Car_Forward(350);//ǰ��

    if (Dis <= 15)
    {
        tmpflag = 1;
        Car_Backward(350);//����
        delay_ms(500);
        Car_TurnLeft(350);//��ת
        delay_ms(500);
    }
    else
    {
        if (tmpflag == 1)
        {
            if (Dis > 150)
            {
                Car_Backward(350);//����
                delay_ms(500);
                Car_TurnLeft(350);//��ת
                delay_ms(500);
            }
        }
        tmpflag = 0;
    }
}

/*------------ҡͷ�����Ż�(�д�������Ч��������)-------------*/
static void deal_dist3(void)
{
    static char tmpflag = 0;	//̫���� ���ä��
    static float disMiddle, disLeft, disRight;
    sgReset();

    disMiddle = Hcsr04GetLength();
    if(disMiddle > 35)
    {
        if(tmpflag == 1 && disMiddle > 150)
        {
			Car_Backward(400);
			delay_ms(1000);
			tmpflag = 0;
        }
		else
		{
			Car_Forward(400);//ǰ��
			delay_ms(400);
		}
        
    }
    else if(disMiddle < 15)
    {
        tmpflag = 1;
        Car_Backward(400);
        delay_ms(400);
    }
    else
    {
        Car_Stop();		//ֹͣ
        /*------------�����-------------*/
        sgLeft();
        servo_dir = LIFT;
        delay_ms(300);
        disLeft = Hcsr04GetLength();
        /*------------���ұ�-------------*/
        sgRight();
        servo_dir = RIGHT;
        delay_ms(300);
        disRight = Hcsr04GetLength();
        /*------------����-------------*/
        if(disLeft < disRight)
        {
            Car_TurnRight(400);
            delay_ms(1000);
            Car_Stop();
        }
        if(disRight < disLeft)
        {
            Car_TurnLeft(400);
            delay_ms(1000);
            Car_Stop();
        }
    }
    delay_ms(50);
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
	sgReset();
	
    //��ȡRoll
    for (i = 1; i < 20; i++)
    {
        if (Pitch_Roll_RxPacket[i] == '.')break;
        Roll_Buf[i - 1] = Pitch_Roll_RxPacket[i];
    }
    //��ȡPitch
    for (i = 0; i < 20; i++)
    {
        if (Pitch_Roll_RxPacket[i] == ',')
        {
            Pitch_flag = 1;
            i++;
        }
        if (Pitch_flag == 1)
        {
            if (Pitch_Roll_RxPacket[i] == '.')
            {
                j = 0;
                break;
            }
            Pitch_Buf[j] = Pitch_Roll_RxPacket[i];
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


        /*------------ӳ����PWMռ�ձȷ�Χ-------------*/
        pwm1 = Map(pwm1, -127, 127, -1000, 1000);
        pwm2 = Map(pwm2, -127, 127, -1000, 1000);
        pwm3 = Map(pwm3, -127, 127, -1000, 1000);
        pwm4 = Map(pwm4, -127, 127, -1000, 1000);

        /*------------���˵�ռ�ձȣ���Ϊ�����������-------------*/
        if (pwm1 < 300 && pwm1 >-300)	pwm1 = 0;
        if (pwm2 < 300 && pwm2 >-300)	pwm2 = 0;
        if (pwm3 < 300 && pwm3 >-300)	pwm3 = 0;
        if (pwm4 < 300 && pwm4 >-300)	pwm4 = 0;

        /*------------�����������ռ�ձ�-------------*/
        if (pwm1 > pwm_limit)	pwm1 = pwm_limit;
        if (pwm2 > pwm_limit)	pwm2 = pwm_limit;
        if (pwm3 > pwm_limit)	pwm3 = pwm_limit;
        if (pwm4 > pwm_limit)	pwm4 = pwm_limit;

        /*------------���Ʒ������ռ�ձ�-------------*/
        if (pwm1 < -pwm_limit)	pwm1 = -pwm_limit;
        if (pwm2 < -pwm_limit)	pwm2 = -pwm_limit;
        if (pwm3 < -pwm_limit)	pwm3 = -pwm_limit;
        if (pwm4 < -pwm_limit)	pwm4 = -pwm_limit;

        /*------------������B-------------*/
        if (pwm1 >= 0)		//B����ת
        {
            MotorB_SetSpeed(1, pwm1);
        }
        else if (pwm1 < 0)	//B�ַ�ת
        {
            MotorB_SetSpeed(0, -pwm1);
        }

        /*------------������C-------------*/
        if (pwm2 >= 0)
        {
            MotorC_SetSpeed(1, pwm2);
        }
        else if (pwm2 < 0)
        {
            MotorC_SetSpeed(0, -pwm2);
        }

        /*------------������D-------------*/
        if (pwm3 >= 0)
        {
            MotorD_SetSpeed(1, pwm3);
        }
        else if (pwm3 < 0)
        {
            MotorD_SetSpeed(0, -pwm3);
        }

        /*------------������A-------------*/
        if (pwm4 >= 0)
        {
            MotorA_SetSpeed(1, pwm4);
        }
        else if (pwm4 < 0)
        {
            MotorA_SetSpeed(0, -pwm4);
        }

        memset(Smoothing_Pitch_Buf, 0, sizeof(Smoothing_Pitch_Buf));
        memset(Smoothing_Roll_Buf, 0, sizeof(Smoothing_Roll_Buf));
        delay_ms(1);
    }

    memset(Roll_Buf, 0, 10);
    memset(Pitch_Buf, 0, 10);

    delay_ms(1);
}

