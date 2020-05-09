#ifndef __CAP_KEY_H
#define __CAP_KEY_H
#include "include.h"

void TPAD_Reset(void);
void TIM2_CH1_Cap_Init(u32 arr,u16 psc);
void TPAD_Init(void);
u8 TPAD_Scan(void);
uint32_t TPAD_Get_MaxVal(void);
uint32_t TPAD_Get_MinVal(void);

#endif

