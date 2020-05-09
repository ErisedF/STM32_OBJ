#include "include.h"





void TIM5_CH1_Cap_Init(void)
{
	/******************************************************************
	定义结构体
	******************************************************************/
	GPIO_InitTypeDef GPIO_InitStructure;								//GPIO设置
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;			//定时器设置
	TIM_ICInitTypeDef  TIM5_ICInitStructure;						//输入捕获设置
	NVIC_InitTypeDef NVIC_InitStructure;								//中断设置
	
	/******************************************************************
	使能时钟
	******************************************************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);		//TIM5
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		//GPIO

	/******************************************************************
	复用设置
	******************************************************************/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	
	/******************************************************************
	GPIO设置
	******************************************************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//GPIO_InitStructure.GPIO_OType
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	/******************************************************************
	定时器设置
	******************************************************************/
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseStructure.TIM_Period = 499;												//计数值
	TIM_TimeBaseStructure.TIM_Prescaler = 83;											//分频系数
	//TIM_TimeBaseStructure.TIM_RepetitionCounter
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	
	/******************************************************************
	输入捕获设置
	******************************************************************/
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1;										//定时器通道
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;														//滤波长度
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;				//上升沿捕获
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;							//输入分频系数
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;		//映射到TI1
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	
	
	/******************************************************************
	使能定时器，中断
	******************************************************************/
	TIM_Cmd(TIM5, ENABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);
	
	/******************************************************************
	中断设置
	******************************************************************/
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

uint8_t  FLAG_CAPTURE_COMPLETE = 0;		//捕获完成标志
uint8_t  FLAG_LEAVE = 1;							//高电平标志
ITStatus	FLAG_CC1IF;									//捕获/比较1中断标志
ITStatus	FLAG_UIF;										//更新中断标志

uint8_t		FLAG_COUNTRT_OF;						//计数器溢出标志		

void TIM5_IRQHandler(void)
{
	FLAG_CC1IF = TIM_GetITStatus(TIM5, TIM_IT_CC1);
	FLAG_UIF = TIM_GetITStatus(TIM5, TIM_IT_Update);
	
	
	if(FLAG_CAPTURE_COMPLETE == RESET)			//没有完成捕获
	{
		
	/******************************************************************
	判断是否更新中断
	******************************************************************/
		if(FLAG_UIF == SET)
		{
			
		}
		
	/******************************************************************
	判断是否捕获中断
	******************************************************************/
		if(FLAG_CC1IF == SET)
		{
			if(FLAG_LEAVE == 1)				//判断是否高电平
			{
				
			}
		}
	}
	
	
	
	
}
