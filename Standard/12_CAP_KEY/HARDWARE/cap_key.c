#include "include.h"

#define TPAD_ARR_MAX_VAL	0XFFFFFFFF		//ARR���ֵ
#define TPAD_GATE_VAL 	100	//����ֵ

__IO uint32_t Counter_CS;
__IO uint32_t Counter_CX;
__IO uint32_t abs_Counter;
/******************************************************************
���ݰ�����λ
�ͷŵ��ݵ�������ʱ������ֵ����
******************************************************************/
void TPAD_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//PA5	���������Ϊ0���ͷŵ���
 	GPIO_InitStructure.GPIO_Pin 		= 	GPIO_Pin_5; 				//GPIOA5
	GPIO_InitStructure.GPIO_Mode 		= 	GPIO_Mode_OUT;				//���ù���
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP; 			//���츴�����
	GPIO_InitStructure.GPIO_PuPd 		= 	GPIO_PuPd_DOWN;		//���������� 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);//���0,�ŵ�
	
	delay_ms(5);
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־
	TIM_SetCounter(TIM2,0);		//��������0
	
	//PA5���գ����ݳ��
	GPIO_InitStructure.GPIO_Pin 		= 	GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode 		= 	GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd 		= 	GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
}


/******************************************************************
���벶������
��ʱ��2 ͨ��1
******************************************************************/
void TIM2_CH1_Cap_Init(u32 arr,u16 psc)
{
	//����ṹ��
	GPIO_InitTypeDef  GPIO_InitStructure; 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM2_ICInitStructure;
	
	//ʹ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	  //TIM2ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	
	//PA5��������
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2); //GPIOA5����λ��ʱ��2
	
	//PA5����
 	GPIO_InitStructure.GPIO_Pin 		= 	GPIO_Pin_5; 				//GPIOA5
	GPIO_InitStructure.GPIO_Mode 		= 	GPIO_Mode_AF;				//���ù���
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP; 			//���츴�����
	GPIO_InitStructure.GPIO_PuPd 		= 	GPIO_PuPd_NOPULL;		//���������� 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	//��ʱ��2����
	TIM_TimeBaseStructure.TIM_ClockDivision 	=  	TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode			=		TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period 					=		arr;
	TIM_TimeBaseStructure.TIM_Prescaler 			=		psc;
	//TIM_TimeBaseStructure.TIM_RepetitionCounter 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	//ͨ��1����
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1; 							//CC1S=01 	ѡ������� IC1ӳ�䵽TIM2��
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//�����ز���
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 				//���������Ƶ,����Ƶ 
  TIM2_ICInitStructure.TIM_ICFilter = 0x00;												//IC2F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM2, &TIM2_ICInitStructure);												//��ʼ��TIM2 IC1
	
	//ʹ�ܶ�ʱ��2
	TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��2

}


/******************************************************************
�õ���ʱ������ֵ
******************************************************************/
uint32_t TPAD_Get_Val(void)
{				   
	TPAD_Reset();
	while(TIM_GetFlagStatus(TIM2, TIM_IT_CC1) == RESET)//�ȴ�����������
	{
		if(TIM_GetCounter(TIM2)>TPAD_ARR_MAX_VAL-500)return TIM_GetCounter(TIM2);//��ʱ��,ֱ�ӷ���CNT��ֵ
	};	
	return TIM_GetCapture1(TIM2);	 //���ز���ֵ
} 	  

/******************************************************************
���ݰ�����ʼ��
******************************************************************/
void TPAD_Init(void)
{
	TIM2_CH1_Cap_Init(TPAD_ARR_MAX_VAL,7);
	Counter_CS = TPAD_Get_MinVal();
}
/******************************************************************
��ȡ���ֵ
******************************************************************/
uint32_t TPAD_Get_MaxVal(void)
{
	uint32_t temp=0;
	uint32_t res=0; 
	u8 n = 8;
	while(n--)
	{
		temp = TPAD_Get_Val();			//�õ�һ��ֵ
		if(temp>res)res=temp;
	};
	return res;
}
/******************************************************************
��ȡ��Сֵ
******************************************************************/
uint32_t TPAD_Get_MinVal(void)
{
	uint32_t temp=0;
	uint32_t res=0xFFFFFFFF; 
	u8 n = 8;
	while(n--)
	{
		temp = TPAD_Get_Val();			//�õ�һ��ֵ
		if(temp<res)res=temp;
	};
	return res;
}
/******************************************************************
ɨ����ݰ���
******************************************************************/
u8 TPAD_Scan(void)
{
	u8 res = 0;
	Counter_CX = TPAD_Get_MaxVal();
	if(Counter_CX >= Counter_CS)
	{
		abs_Counter = Counter_CX - Counter_CS;
	}
	else 
	{
		abs_Counter = Counter_CS - Counter_CX;
	}
	
	if(abs_Counter >= TPAD_GATE_VAL)
	{
		res = 1;
	}
	else 
	{
		res = 0;
	}
	return res;
}



