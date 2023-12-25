#ifndef _MYMECANUM_H_
#define _MYMECANUM_H_

#include "stm32f10x.h"

void car_move(int LR, int FB, int turn);
void motor_control(char motorX, int ref_speed);

#endif

