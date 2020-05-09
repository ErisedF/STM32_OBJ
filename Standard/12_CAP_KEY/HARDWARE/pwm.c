#include "include.h"




void PWM_TIM14_Init(void)
{
	/******************************************************************
	定义结构体
	******************************************************************/
	GPIO_InitTypeDef GPIO_InitStructure;								//GPIO
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;			//时钟
	TIM_OCInitTypeDef  TIM_OCInitStructure;							//PWM通道

	/******************************************************************
	使能时钟
	******************************************************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/******************************************************************
	复用设置
	******************************************************************/
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	
	/******************************************************************
	GPIO设置
	******************************************************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	/******************************************************************
	定时器设置
	******************************************************************/
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 499;
	TIM_TimeBaseStructure.TIM_Prescaler = 83;
//	TIM_TimeBaseStructure.TIM_RepetitionCounter
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
	
	/******************************************************************
	PWM设置（输出比较，输入捕获）
	******************************************************************/
//	TIM_OCInitStructure.TIM_OCIdleState = 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;					//输出模式
//	TIM_OCInitStructure.TIM_OCNIdleState = 
//	TIM_OCInitStructure.TIM_OCNPolarity = 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;	//输出极性
//	TIM_OCInitStructure.TIM_OutputNState  
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
//	TIM_OCInitStructure.TIM_Pulse 
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);
	
	/******************************************************************
	使能预装载寄存器
	******************************************************************/
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);		//使能预装载寄存器
	TIM_ARRPreloadConfig(TIM14, ENABLE);									//使能ARR
	TIM_Cmd(TIM14, ENABLE);																//使能定时器
}




