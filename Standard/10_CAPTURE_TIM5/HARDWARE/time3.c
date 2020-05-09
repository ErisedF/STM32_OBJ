#include "include.h"


	/******************************************************************
	TIMER3�жϷ��񺫺���
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
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//����ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//ʹ��ʱ��
	
	/******************************************************************
	TIMER3��ʼ��
	******************************************************************/
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;				//���벶���Ƶ����
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = 4999;											//�Զ����ؼ�������
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0x20cf;									//��Ƶϵ��
	//TIM_TimeBaseInitStructure.TIM_RepetitionCounter 								//�߼���ʱ����
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	
	/******************************************************************
	TIMER3�ж�����
	******************************************************************/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);				//��ʱ��3�����ж�
	TIM_Cmd(TIM3, ENABLE);														//ʹ�ܶ�ʱ���ж�
	/******************************************************************
	�ж�����
	******************************************************************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}
