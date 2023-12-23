#ifndef __JOYSTICK_H
#define __JOYSTICK_H	
#include "sys.h" 
#include "adc.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "nrf24l01.h"
#include "oled.h"


/****************************************************************************       
               (330,000)   (165,000)   (000,000)
     ___ 
		|Key|	     (330,165)   (165,165)   (000,165)
		
               (330,330)   (165,330)   (000,330)

****************************************************************************/

#define L_UP      0
#define L_DOWN    1
#define L_LEFT    2
#define L_RIGHT   3

#define L_L_UP    4
#define L_R_UP    5
#define L_L_DOWN  6
#define L_R_DOWN  7

#define R_UP      8
#define R_DOWN    9
#define R_LEFT    10
#define R_RIGHT   11

#define R_L_UP    12
#define R_R_UP    13
#define R_L_DOWN  14
#define R_R_DOWN  15

#define L_STOP    16
#define R_STOP    17

#define KEY_Z0 PBin(0)
#define KEY_Z1 PBin(1)

int Map(int val,int in_min,int in_max,int out_min,int out_max);

void JoyStick_ADC_Init(void);
void JoyStick_DMA_Init(void);

void JoyStick_Key_Init(void);
int JoyStick_Key_Scan(int mode);
void JoyStick_Init(void);
void JoyStick_Text(void);
void JoyStick_Data(void);
void JoyStick_OledShow(void);
void JoyStick_DirSend(void);


#endif


