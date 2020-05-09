#ifndef __KEY_H
#define __KEY_H
#include "stm32f4xx.h"                  // Device header
#include "delay.h"

#define KEY_UP GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
//#define KEY_0 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)
#define KEY_1 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define KEY_2 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)



void Key_Init(void);
u8 Key_Scan(u8 mode);

#endif

