#include "include.h"


	/******************************************************************
	TIMER3中断服务韩函数
	******************************************************************/
	void TIM3_IRQHandler(void)
	{
		if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
		{
			LED1 = !LED1;
		}
		TIM_ClearFlag(TIM3, TIM_IT_Update);
	}
	
	

void TIMER3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//使能时钟
	
	/******************************************************************
	TIMER3初始化
	******************************************************************/
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;				//输入捕获分频因子
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 4999;											//自动重载计数周期
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0x20cf;									//分频系数
	//TIM_TimeBaseInitStructure.TIM_RepetitionCounter 								//高级定时器用
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	
	/******************************************************************
	TIMER3中断设置
	******************************************************************/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);				//定时器3更新中断
	TIM_Cmd(TIM3, ENABLE);														//使能定时器中断
	/******************************************************************
	中断设置
	******************************************************************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}
