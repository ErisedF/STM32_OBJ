#include "include.h"

#define TPAD_ARR_MAX_VAL	0XFFFFFFFF		//ARR最大值
#define TPAD_GATE_VAL 	100	//门限值

__IO uint32_t Counter_CS;
__IO uint32_t Counter_CX;
__IO uint32_t abs_Counter;
/******************************************************************
电容按键复位
释放电容电量，定时器计数值清零
******************************************************************/
void TPAD_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//PA5	推推挽输出为0，释放电量
 	GPIO_InitStructure.GPIO_Pin 		= 	GPIO_Pin_5; 				//GPIOA5
	GPIO_InitStructure.GPIO_Mode 		= 	GPIO_Mode_OUT;				//复用功能
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP; 			//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd 		= 	GPIO_PuPd_DOWN;		//不带上下拉 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);//输出0,放电
	
	delay_ms(5);
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //清除中断标志
	TIM_SetCounter(TIM2,0);		//计数器归0
	
	//PA5悬空，电容充电
	GPIO_InitStructure.GPIO_Pin 		= 	GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode 		= 	GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd 		= 	GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
}


/******************************************************************
输入捕获设置
定时器2 通道1
******************************************************************/
void TIM2_CH1_Cap_Init(u32 arr,u16 psc)
{
	//定义结构体
	GPIO_InitTypeDef  GPIO_InitStructure; 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM2_ICInitStructure;
	
	//使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	  //TIM2时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	//PA5复用设置
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2); //GPIOA5复用位定时器2
	
	//PA5设置
 	GPIO_InitStructure.GPIO_Pin 		= 	GPIO_Pin_5; 				//GPIOA5
	GPIO_InitStructure.GPIO_Mode 		= 	GPIO_Mode_AF;				//复用功能
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP; 			//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd 		= 	GPIO_PuPd_NOPULL;		//不带上下拉 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	//定时器2设置
	TIM_TimeBaseStructure.TIM_ClockDivision 	=  	TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode			=		TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period 					=		arr;
	TIM_TimeBaseStructure.TIM_Prescaler 			=		psc;
	//TIM_TimeBaseStructure.TIM_RepetitionCounter 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	//通道1设置
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1; 							//CC1S=01 	选择输入端 IC1映射到TIM2上
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//上升沿捕获
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 				//配置输入分频,不分频 
  TIM2_ICInitStructure.TIM_ICFilter = 0x00;												//IC2F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM2, &TIM2_ICInitStructure);												//初始化TIM2 IC1
	
	//使能定时器2
	TIM_Cmd(TIM2,ENABLE ); 	//使能定时器2

}


/******************************************************************
得到定时器捕获值
******************************************************************/
uint32_t TPAD_Get_Val(void)
{				   
	TPAD_Reset();
	while(TIM_GetFlagStatus(TIM2, TIM_IT_CC1) == RESET)//等待捕获上升沿
	{
		if(TIM_GetCounter(TIM2)>TPAD_ARR_MAX_VAL-500)return TIM_GetCounter(TIM2);//超时了,直接返回CNT的值
	};	
	return TIM_GetCapture1(TIM2);	 //返回捕获值
} 	  

/******************************************************************
电容按键初始化
******************************************************************/
void TPAD_Init(void)
{
	TIM2_CH1_Cap_Init(TPAD_ARR_MAX_VAL,7);
	Counter_CS = TPAD_Get_MinVal();
}
/******************************************************************
获取最大值
******************************************************************/
uint32_t TPAD_Get_MaxVal(void)
{
	uint32_t temp=0;
	uint32_t res=0; 
	u8 n = 8;
	while(n--)
	{
		temp = TPAD_Get_Val();			//得到一次值
		if(temp>res)res=temp;
	};
	return res;
}
/******************************************************************
获取最小值
******************************************************************/
uint32_t TPAD_Get_MinVal(void)
{
	uint32_t temp=0;
	uint32_t res=0xFFFFFFFF; 
	u8 n = 8;
	while(n--)
	{
		temp = TPAD_Get_Val();			//得到一次值
		if(temp<res)res=temp;
	};
	return res;
}
/******************************************************************
扫描电容按键
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



