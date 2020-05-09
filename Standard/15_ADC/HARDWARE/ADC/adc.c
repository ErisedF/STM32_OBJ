#include "adc.h"
#include "delay.h"		 


/******************************************************************
ADC初始化
******************************************************************/														   
void  Adc_Init(void)
{
/******************************************************************
											定义结构体
	GPIO
	ADC通用初始化
	ADC初始化
******************************************************************/	
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
/******************************************************************
											使能时钟  PA5
	GPIOA
	ADC1
******************************************************************/	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 	//使能ADC1时钟

  //初始化ADC1通道5 IO口
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;				//PA5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;			//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;	//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);						//初始化  
	//ADC1复位
//ADC_DeInit();
//	{
//  /* Enable all ADCs reset state */
//  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC, ENABLE);
//  
//  /* Release all ADCs from reset state */
//  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC, DISABLE);
//	};
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 
	//通用初始化
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;											//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//两个采样阶段之间的延迟5个周期
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 			//DMA不使能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;										//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟不超过36MHZ
  ADC_CommonInit(&ADC_CommonInitStructure);																			//初始化
	//具体ADC初始化
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;		//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;							//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;				//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	//ADC_InitStructure.ADC_ExternalTrigConv //软件触发，不设置
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;								//转换通道数量 1
  ADC_Init(ADC1, &ADC_InitStructure);												//ADC初始化
	//使能ADC1
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	

}				  

/******************************************************************
获得ADC值
ch: @ref ADC_channels 
通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
返回值:转换结果
******************************************************************/
u16 Get_Adc(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig
													(
														 ADC1,   //指定ADC
														 ch,	 //指定通道
														 1, 	 //指定在通道组中第几个转换
														 ADC_SampleTime_84Cycles 	 
													);  
  
	ADC_SoftwareStartConv(ADC1);										//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));	//等待转换结束

	return ADC_GetConversionValue(ADC1);						//返回最近一次ADC1规则组的转换结果
}

/******************************************************************
获取通道ch的转换值，取times次,然后平均 
ch:通道编号
times:获取次数
返回值:通道ch的times次转换结果平均值
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
	 









