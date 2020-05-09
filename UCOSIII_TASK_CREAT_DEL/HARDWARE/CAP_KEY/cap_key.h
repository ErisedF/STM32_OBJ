#ifndef __CAP_KEY_H
#define __CAP_KEY_H
#include "sys.h"
#include "delay.h"

//PA5输出低电平放电，计数器值清空
void TPAD_Reset(void);
//定时器2通道1输入捕获设置
void TIM2_CH1_Cap_Init(u32 arr,u16 psc);
//电容按键初始化
void TPAD_Init(void);
//电容按键值扫描
u8 TPAD_Scan(void);
//获取电容按键最大值最小值
uint32_t TPAD_Get_MaxVal(void);
uint32_t TPAD_Get_MinVal(void);
uint32_t TPAD_Get_Val(void);

#endif

