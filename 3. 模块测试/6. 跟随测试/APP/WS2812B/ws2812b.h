#ifndef __WS2812B_H
#define __WS2812B_H
#include "sys.h"
#include "delay.h"


#define 	RGB_LED 	GPIO_Pin_0
#define		RGB_LED_HIGH	(GPIO_SetBits(GPIOB,RGB_LED))
#define 	RGB_LED_LOW		(GPIO_ResetBits(GPIOB,RGB_LED))



void RGB_LED_Init(void);
void RGB_LED_Write0(void);
void RGB_LED_Write1(void);
void RGB_LED_Reset(void);
void RGB_LED_Write_Byte(uint8_t byte);
void RGB_LED_Write_24Bits(uint8_t green,uint8_t red,uint8_t blue);
void RGB_LED_Red(void);
void RGB_LED_Green(void);
void RGB_LED_Blue(void);
void RGB_Circulation(u8 mode);
void hsv_to_rgb(int h,int s,int v,float *R,float *G,float *B);


#endif



