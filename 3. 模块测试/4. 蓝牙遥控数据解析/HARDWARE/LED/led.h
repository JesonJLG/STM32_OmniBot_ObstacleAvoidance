#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"                  // Device header

#define pinLED1		GPIO_Pin_0
#define pinLED2		GPIO_Pin_1

void LED_Init(void);//LED≥ı ºªØ
void LED_ON(uint16_t pin);
void LED_OFF(uint16_t pin);


#endif
