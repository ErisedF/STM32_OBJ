#include "adc.h"
#include "delay.h"		 


/******************************************************************
ADC��ʼ��
******************************************************************/														   
void  Adc_Init(void)
{
/******************************************************************
											����ṹ��
	GPIO
	ADCͨ�ó�ʼ��
	ADC��ʼ��
******************************************************************/	
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
/******************************************************************
											ʹ��ʱ��  PA5
	GPIOA
	ADC1
******************************************************************/	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 	//ʹ��ADC1ʱ��

  //��ʼ��ADC1ͨ��5 IO��
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;				//PA5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;			//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;	//����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);						//��ʼ��  
	//ADC1��λ
//ADC_DeInit();
//	{
//  /* Enable all ADCs reset state */
//  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC, ENABLE);
//  
//  /* Release all ADCs from reset state */
//  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC, DISABLE);
//	};
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
	//ͨ�ó�ʼ��
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;											//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//���������׶�֮����ӳ�5������
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 			//DMA��ʹ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;										//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ�Ӳ�����36MHZ
  ADC_CommonInit(&ADC_CommonInitStructure);																			//��ʼ��
	//����ADC��ʼ��
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;		//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;							//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;				//�ر�����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ����������
	//ADC_InitStructure.ADC_ExternalTrigConv //����������������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;								//ת��ͨ������ 1
  ADC_Init(ADC1, &ADC_InitStructure);												//ADC��ʼ��
	//ʹ��ADC1
	ADC_Cmd(ADC1, ENABLE);//����ADת����	

}				  

/******************************************************************
���ADCֵ
ch: @ref ADC_channels 
ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
����ֵ:ת�����
******************************************************************/
u16 Get_Adc(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig
													(
														 ADC1,   //ָ��ADC
														 ch,	 //ָ��ͨ��
														 1, 	 //ָ����ͨ�����еڼ���ת��
														 ADC_SampleTime_84Cycles 	 
													);  
  
	ADC_SoftwareStartConv(ADC1);										//ʹ��ָ����ADC1������ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));	//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);						//�������һ��ADC1�������ת�����
}

/******************************************************************
��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
ch:ͨ�����
times:��ȡ����
����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
******************************************************************/
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(1);
	}
	return temp_val/times;
} 
	 








