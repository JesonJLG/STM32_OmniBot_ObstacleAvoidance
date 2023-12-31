#ifndef __COUNT_SENSOR_H
#define __COUNT_SENSOR_H

#include "stm32f10x.h"                  // Device header

#define CNTSSR_PORT	GPIOB
#define CNTSSR_DO	GPIO_Pin_4
#define CNTSSR_EXTI	EXTI_Line4

void CountSensor_Init(void);
uint16_t CountSensor_Get(void);

#endif
