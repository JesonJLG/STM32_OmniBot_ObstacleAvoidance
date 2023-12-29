#ifndef __CONTROL_H
#define __CONTROL_H	 
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
#include "nrf24l01.h"
#include "hcsr.h"
#include "tcrt.h"
#include "joystick.h"
#include "math.h"
#include "string.h"
#include "ws2812b.h"


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


extern u8 flag;
extern float RGB_R,RGB_G,RGB_B;//HSV×ªRGBµÄÑÕÉ«
extern char Pitch_Roll_Buf[20];


int Map(int val,int in_min,int in_max,int out_min,int out_max);
void APP_Joy_Mode(void);
void APP_Gravity_Mode(void);

void Bluetooth_Mode(void);
void Wireless_Mode(void);
void Evadible_Mode(void);
void Gravity_Mode(void);
void Follow_Mode(void);
void RGB_Select(void);
void RGB_Show(void);
void Control(void);

		 				    
#endif



