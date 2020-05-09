#ifndef _KEY_LED_BEEP_H
#define _KEY_H

#include "stdint.h"
#include "gpio.h"

#define KEY0        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)  //KEY0����PE4
#define KEY1        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)  //KEY1����PE3
#define KEY2        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2) 	//KEY2����PE2
#define WK_UP       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //WKUP����PA0

#define LED0				HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
#define LED1				HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10);
#define BEEP				HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_11);


#define KEY0_PRES 	1
#define KEY1_PRES	2
#define KEY2_PRES	3
#define WKUP_PRES   4

//void KEY_Init(void);
uint8_t KEY_Scan(uint8_t mode);
#endif
