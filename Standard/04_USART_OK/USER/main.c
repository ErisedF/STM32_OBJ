#include "stm32f4xx.h"

	/******************************************************************
	�жϴ�������
	******************************************************************/
void USART1_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		res = USART_ReceiveData(USART1);
		USART_SendData(USART1, res);
	}
}



int main(void)
{
	
	/******************************************************************
	����ṹ��
	******************************************************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	/******************************************************************
	ʹ��ʱ��
	******************************************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	
	/******************************************************************
	GPIO��������
	******************************************************************/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);


	/******************************************************************
	GPIO��������
	******************************************************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/******************************************************************
	USART��������
	******************************************************************/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1, &USART_InitStructure);
	
	
	/******************************************************************
	ʹ�ܴ���
	******************************************************************/
	USART_Cmd(USART1, ENABLE);
	
	/******************************************************************
	�����ж����ȼ������ʹ��
	******************************************************************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
	/******************************************************************
	�����ж�����
	******************************************************************/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	
	while(1);
}