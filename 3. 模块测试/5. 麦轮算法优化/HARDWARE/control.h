#ifndef __CONTROL_H
#define __CONTROL_H

#include "stm32f10x.h"                  // Device header

#define MPU_D      0
#define MPU_U      1

#define MPU_L      2
#define MPU_R      3

#define MPU_L_D    4
#define MPU_R_D    5
#define MPU_L_U    6
#define MPU_R_U    7

#define MPU_STOP   10

#define  LY_Mode 1
#define  YK_Mode 2
#define  ZL_Mode 3
#define  BZ_Mode 4
#define  GS_Mode 5
#define  XJ_Mode 6

int Map(int val,int in_min,int in_max,int out_min,int out_max);
void APP_Joy_Mode(void);
void APP_Gravity_Mode(void);

void Bluetooth_Mode(void);
void Evadible_Mode(void);
void Follow_Mode(void);
void Control(void);

#endif



