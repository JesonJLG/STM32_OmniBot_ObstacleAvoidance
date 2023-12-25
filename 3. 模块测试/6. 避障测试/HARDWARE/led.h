#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"                  // Device header

#define LED_PORT	GPIOA
#define LED1_PIN	GPIO_Pin_0
#define LED2_PIN	GPIO_Pin_1

void LED_Init(void);//LED≥ı ºªØ
void LED_ON(uint16_t PIN);
void LED_OFF(uint16_t PIN);


#endif
