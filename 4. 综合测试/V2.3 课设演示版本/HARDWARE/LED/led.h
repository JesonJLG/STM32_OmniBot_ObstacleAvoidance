#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0_PORT	GPIOB		//DS0
#define LED0_PIN	GPIO_Pin_5	
#define LED1_PORT	GPIOE		//DS1
#define LED1_PIN	GPIO_Pin_5	

#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	

void LED_Init(void);


#endif
