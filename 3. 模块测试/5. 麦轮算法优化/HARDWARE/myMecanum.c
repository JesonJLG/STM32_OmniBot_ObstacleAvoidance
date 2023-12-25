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
    float k;			//缩放因子
    int Va, Vb, Vc, Vd;

    move_sum = abs(FB) + abs(LR) + abs(turn);
//    printf("move_sum:%d\r\n", move_sum);

    if(move_sum >= 100)		//当速度和的绝对值过大时
    {
        k = 100.0/move_sum;	//为了保证速度比
        FB = FB * k;		//等比缩小所有数值
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

    motor_control('A', Va);	//左后电机A
    motor_control('B', Vb);	//左前电机B
    motor_control('C', Vc);	//右前电机C
    motor_control('D', Vd);	//右后电机D

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
        if(pwm >= 0)	//视觉上的正转
        {
            TIM_SetCompare4(timMOTOR_AB,0);
            TIM_SetCompare3(timMOTOR_AB,pwm);
        }
        else				//视觉上的反转
        {
            TIM_SetCompare3(timMOTOR_AB,0);
            TIM_SetCompare4(timMOTOR_AB,-pwm);
        }
        break;
    case 'B':
        if(pwm >= 0)	//视觉上的正转
        {
            TIM_SetCompare2(timMOTOR_AB,0);
            TIM_SetCompare1(timMOTOR_AB,pwm);
        }
        else				//视觉上的反转
        {
            TIM_SetCompare1(timMOTOR_AB,0);
            TIM_SetCompare2(timMOTOR_AB,-pwm);
        }
        break;
    case 'C':
        if(pwm >= 0)	//视觉上的正转
        {
            TIM_SetCompare3(timMOTOR_CD,0);
            TIM_SetCompare4(timMOTOR_CD,pwm);
        }
        else				//视觉上的反转
        {
            TIM_SetCompare4(timMOTOR_CD,0);
            TIM_SetCompare3(timMOTOR_CD,-pwm);
        }
        break;
    case 'D':
        if(pwm >= 0)	//视觉上的正转
        {
            TIM_SetCompare1(timMOTOR_CD,0);
            TIM_SetCompare2(timMOTOR_CD,pwm);
        }
        else				//视觉上的反转
        {
            TIM_SetCompare2(timMOTOR_CD,0);
            TIM_SetCompare1(timMOTOR_CD,-pwm);
        }
        break;
    default:
        printf("该电机不存在\r\n");
        break;
    }
}

