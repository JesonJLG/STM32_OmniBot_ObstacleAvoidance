#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define LED0_PORT   GPIOC
#define LED0_PIN    GPIO_Pin_13

#define LED0 PCout(13)

void LED_Init(void);


#endif
