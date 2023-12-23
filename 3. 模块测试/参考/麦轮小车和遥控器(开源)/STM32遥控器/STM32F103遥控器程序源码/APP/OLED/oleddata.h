#ifndef __OLEDDATA_H
#define __OLEDDATA_H	
#include "sys.h" 
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "oled.h"
#include "nrf24l01.h"
#include "joystick.h"
#include "mpu6050.h"
#include "snake.h"
#include "timer.h"
#include "string.h"

#define  LY_Mode  1
#define  YK_Mode  2
#define  ZL_Mode  3
#define  BZ_Mode  4
#define  GS_Mode  5
#define  RGB_Mode  6
#define  TCS_Mode 7

extern int Joy_Lx,Joy_Ly,Joy_Rx,Joy_Ry;
extern u16 L_Dir,R_Dir;
extern int Key_flag1,Key_flag2;
extern u8 MPU_TxBuf[6];
extern float pitch,roll,yaw;
extern int map[32][12];//地图大小
extern int snake_Grid[32][2];
extern int direction;//蛇的方向

void OLED_Battery(void);
void OLED_Progress(void);
void OLED_Face(void);
void OLED_Control(void);
void OLED_ShowMove_Icon(u8 mode,u8 Dir);
void OLED_Roll(u8 mode,u8 dir);
void OLED_Mode(u8 mode);

	
#endif


