#ifndef __LED_BEEP_H
#define __LED_BEEP_H


#include "include.h"


#define BEEP GPIO_ReadOutputDataBit(GPIOF,GPIO_Pin_8)
#define LED_0 GPIO_ReadOutputDataBit(GPIOF,GPIO_Pin_9)
#define LED_1 GPIO_ReadOutputDataBit(GPIOF,GPIO_Pin_10)


void LED_BEEP_Init(void);


#endif
