#include "include.h"




void PWM_TIM14_Init(void)
{
	/******************************************************************
	����ṹ��
	******************************************************************/
	GPIO_InitTypeDef GPIO_InitStructure;								//GPIO
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;			//ʱ��
	TIM_OCInitTypeDef  TIM_OCInitStructure;							//PWMͨ��

	/******************************************************************
	ʹ��ʱ��
	******************************************************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/******************************************************************
	��������
	******************************************************************/
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	
	/******************************************************************
	GPIO����
	******************************************************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	/******************************************************************
	��ʱ������
	******************************************************************/
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 499;
	TIM_TimeBaseStructure.TIM_Prescaler = 83;
//	TIM_TimeBaseStructure.TIM_RepetitionCounter
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
	
	/******************************************************************
	PWM���ã�����Ƚϣ����벶��
	******************************************************************/
//	TIM_OCInitStructure.TIM_OCIdleState = 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;					//���ģʽ
//	TIM_OCInitStructure.TIM_OCNIdleState = 
//	TIM_OCInitStructure.TIM_OCNPolarity = 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;	//�������
//	TIM_OCInitStructure.TIM_OutputNState  
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_Pulse 
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);
	
	/******************************************************************
	ʹ��Ԥװ�ؼĴ���
	******************************************************************/
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);		//ʹ��Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM14, ENABLE);									//ʹ��ARR
	TIM_Cmd(TIM14, ENABLE);																//ʹ�ܶ�ʱ��
}




