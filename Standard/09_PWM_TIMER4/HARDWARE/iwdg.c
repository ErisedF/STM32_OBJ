#include "include.h"



void IWDG_Init(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);		//IWDG写使能
	IWDG_SetPrescaler(IWDG_Prescaler_64);						//设置预分频系数
	IWDG_SetReload(0x4E2);													//设置重载计数值
	IWDG_ReloadCounter();														//装载重载计数值(喂狗)
	IWDG_Enable();																	//使能独立看门狗
}



void IWDG_Free(void)
{
	IWDG_ReloadCounter();														//装载重载计数值(喂狗)
}
