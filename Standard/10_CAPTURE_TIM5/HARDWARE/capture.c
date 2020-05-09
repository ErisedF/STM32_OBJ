#include "include.h"





void TIM5_CH1_Cap_Init(void)
{
	/******************************************************************
	����ṹ��
	******************************************************************/
	GPIO_InitTypeDef GPIO_InitStructure;								//GPIO����
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;			//��ʱ������
	TIM_ICInitTypeDef  TIM5_ICInitStructure;						//���벶������
	NVIC_InitTypeDef NVIC_InitStructure;								//�ж�����
	
	/******************************************************************
	ʹ��ʱ��
	******************************************************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);		//TIM5
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		//GPIO

	/******************************************************************
	��������
	******************************************************************/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	
	/******************************************************************
	GPIO����
	******************************************************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//GPIO_InitStructure.GPIO_OType
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	/******************************************************************
	��ʱ������
	******************************************************************/
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���ϼ���
	TIM_TimeBaseStructure.TIM_Period = 499;												//����ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 83;											//��Ƶϵ��
	//TIM_TimeBaseStructure.TIM_RepetitionCounter
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	
	/******************************************************************
	���벶������
	******************************************************************/
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1;										//��ʱ��ͨ��
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;														//�˲�����
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;				//�����ز���
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;							//�����Ƶϵ��
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;		//ӳ�䵽TI1
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	
	
	/******************************************************************
	ʹ�ܶ�ʱ�����ж�
	******************************************************************/
	TIM_Cmd(TIM5, ENABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);
	
	/******************************************************************
	�ж�����
	******************************************************************/
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

uint8_t  FLAG_CAPTURE_COMPLETE = 0;		//������ɱ�־
uint8_t  FLAG_LEAVE = 1;							//�ߵ�ƽ��־
ITStatus	FLAG_CC1IF;									//����/�Ƚ�1�жϱ�־
ITStatus	FLAG_UIF;										//�����жϱ�־

uint8_t		FLAG_COUNTRT_OF;						//�����������־		

void TIM5_IRQHandler(void)
{
	FLAG_CC1IF = TIM_GetITStatus(TIM5, TIM_IT_CC1);
	FLAG_UIF = TIM_GetITStatus(TIM5, TIM_IT_Update);
	
	
	if(FLAG_CAPTURE_COMPLETE == RESET)			//û����ɲ���
	{
		
	/******************************************************************
	�ж��Ƿ�����ж�
	******************************************************************/
		if(FLAG_UIF == SET)
		{
			
		}
		
	/******************************************************************
	�ж��Ƿ񲶻��ж�
	******************************************************************/
		if(FLAG_CC1IF == SET)
		{
			if(FLAG_LEAVE == 1)				//�ж��Ƿ�ߵ�ƽ
			{
				
			}
		}
	}
	
	
	
	
}
