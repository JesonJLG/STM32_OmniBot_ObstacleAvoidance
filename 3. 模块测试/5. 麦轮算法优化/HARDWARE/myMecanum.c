#include "myMecanum.h"
#include "motor.h"
#include "usart.h"
#include "stdlib.h"
#include "control.h"

//enum
//{
//    motorA = 0,
//    motorB,
//    motorC,
//    motorD,
//} motorX;


void car_move(int LR, int FB, int turn)
{
    int move_sum;
    float k;			//��������
    int Va, Vb, Vc, Vd;

    move_sum = abs(FB) + abs(LR) + abs(turn);
//    printf("move_sum:%d\r\n", move_sum);

    if(move_sum >= 100)		//���ٶȺ͵ľ���ֵ����ʱ
    {
        k = 100.0/move_sum;	//Ϊ�˱�֤�ٶȱ�
        FB = FB * k;		//�ȱ���С������ֵ
        LR = LR * k;
        turn = turn * k;

//		printf("k:%.2f	\r\n", k);
//		printf("FB:%d	", FB);
//		printf("LR:%d	", LR);
//		printf("turn:%d	", turn);
//		printf("\r\n");
    }

    Va = -FB-LR+turn;
    Vb = -FB+LR+turn;
    Vc = -FB-LR-turn;
    Vd = -FB+LR-turn;

//    printf("Va:%d	", Va);
//    printf("Vb:%d	", Vb);
//    printf("Vc:%d	", Vc);
//    printf("Vd:%d	", Vd);
//    printf("\r\n");

    motor_control('A', Va);	//�����A
    motor_control('B', Vb);	//��ǰ���B
    motor_control('C', Vc);	//��ǰ���C
    motor_control('D', Vd);	//�Һ���D

}

void motor_control(char motorX, int ref_speed)
{
    int pwm;

    pwm = Map(ref_speed,-100,100,-1000,1000);
	printf("motorX:%c	ref_speed:%d	pwm:%d",motorX,ref_speed,pwm);
	printf("\r\n");
    switch (motorX)
    {
    case 'A':
        if(pwm >= 0)	//�Ӿ��ϵ���ת
        {
            TIM_SetCompare4(timMOTOR_AB,0);
            TIM_SetCompare3(timMOTOR_AB,pwm);
        }
        else				//�Ӿ��ϵķ�ת
        {
            TIM_SetCompare3(timMOTOR_AB,0);
            TIM_SetCompare4(timMOTOR_AB,-pwm);
        }
        break;
    case 'B':
        if(pwm >= 0)	//�Ӿ��ϵ���ת
        {
            TIM_SetCompare2(timMOTOR_AB,0);
            TIM_SetCompare1(timMOTOR_AB,pwm);
        }
        else				//�Ӿ��ϵķ�ת
        {
            TIM_SetCompare1(timMOTOR_AB,0);
            TIM_SetCompare2(timMOTOR_AB,-pwm);
        }
        break;
    case 'C':
        if(pwm >= 0)	//�Ӿ��ϵ���ת
        {
            TIM_SetCompare3(timMOTOR_CD,0);
            TIM_SetCompare4(timMOTOR_CD,pwm);
        }
        else				//�Ӿ��ϵķ�ת
        {
            TIM_SetCompare4(timMOTOR_CD,0);
            TIM_SetCompare3(timMOTOR_CD,-pwm);
        }
        break;
    case 'D':
        if(pwm >= 0)	//�Ӿ��ϵ���ת
        {
            TIM_SetCompare1(timMOTOR_CD,0);
            TIM_SetCompare2(timMOTOR_CD,pwm);
        }
        else				//�Ӿ��ϵķ�ת
        {
            TIM_SetCompare2(timMOTOR_CD,0);
            TIM_SetCompare1(timMOTOR_CD,-pwm);
        }
        break;
    default:
        printf("�õ��������\r\n");
        break;
    }
}

