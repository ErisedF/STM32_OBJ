#include "include.h"



	/******************************************************************
	WWDG�жϴ�����
	******************************************************************/
void WWDG_IRQHandler(void)
{
	WWDG_SetCounter(0x7f);	//���ü���ֵ
	WWDG_ClearFlag();				//���Ԥ�жϱ�־λ
	LED1 = !LED1;

}




void WWDG_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;									//�����жϽṹ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);		//ʹ��WWDGʱ��
	
	/******************************************************************
	WWDG��������
	******************************************************************/
	WWDG_SetWindowValue(0x7f);											//���ô���ֵ
	WWDG_SetPrescaler(WWDG_Prescaler_8);						//����Ԥ�ȷ�Ƶϵ��
	WWDG_SetCounter(0x7f);													//���ü���ֵ
	WWDG_Enable(0x7f);															//ʹ��WWDG
	
	
	/******************************************************************
	WWDG�ж�����
	******************************************************************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);								//����ϵͳ�ж����ȼ�����2
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;								//���ÿ��Ź��ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//�ж�ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;			//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;						//��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);
	
	WWDG_ClearFlag();				//���Ԥ�жϱ�־λ
	WWDG_EnableIT();				//ʹ��Ԥ�ж�
}

