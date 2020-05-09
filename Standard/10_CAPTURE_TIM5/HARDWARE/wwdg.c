#include "include.h"



	/******************************************************************
	WWDG中断处理函数
	******************************************************************/
void WWDG_IRQHandler(void)
{
	WWDG_SetCounter(0x7f);	//设置计数值
	WWDG_ClearFlag();				//清楚预中断标志位
	LED1 = !LED1;

}




void WWDG_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;									//定义中断结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);		//使能WWDG时钟
	
	/******************************************************************
	WWDG参数设置
	******************************************************************/
	WWDG_SetWindowValue(0x7f);											//设置窗口值
	WWDG_SetPrescaler(WWDG_Prescaler_8);						//设置预先分频系数
	WWDG_SetCounter(0x7f);													//设置计数值
	WWDG_Enable(0x7f);															//使能WWDG
	
	
	/******************************************************************
	WWDG中断设置
	******************************************************************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);								//设置系统中断优先级分组2
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;								//设置看门狗中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//中断使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;			//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;						//相应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	WWDG_ClearFlag();				//清楚预中断标志位
	WWDG_EnableIT();				//使能预中断
}

