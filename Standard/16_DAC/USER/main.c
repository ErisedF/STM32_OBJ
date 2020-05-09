#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "dac.h"
#include "key.h"

//#define SSD_HOR_RESOLUTION		800		//LCD水平分辨率
//#define SSD_VER_RESOLUTION		480		//LCD垂直分辨率
#define N 770
#define RANGE 330.0

//u16	ADC_buffer [N];
u16 Line [4] = {0,0,0,0};
int main(void)
{ 
	u16 dacx;
	u16 adcx;
	
	u16 i; 		//画坐标用
	u16 y = 100; 		//画坐标用
	u16 m = 0;

	float temp;
	float temp_1;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);      //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化
	Adc_Init(); 				//adc初始化
	KEY_Init(); 				//按键初始化
	Dac1_Init();		 		//DAC通道1初始化	
	
	
	LCD_Display_Dir(1);
	POINT_COLOR=BLUE;//设置字体为蓝色  
	LCD_DrawLine(30,0,30,480);		//纵坐标
	LCD_DrawLine(0,450,800,450);	//横坐标
	
	
	POINT_COLOR=RED;//设置字体颜色
	for(i = 7 ; i > 0 ; i-- )
	{
		
		LCD_Draw_Circle(30,y,3);
		LCD_ShowString(8,y,200,16,16,".");	//先在固定位置显示小数点 
		if(i%2==0)
		{
			LCD_ShowxNum(0,y,i/2,1,16,0);
			LCD_ShowxNum(16,y,0,1,16,0);
		}
		else
		{
		LCD_ShowxNum(16,y,5,1,16,0);
		LCD_ShowxNum(0,y,i/2,1,16,0);
			
		}		
		y = y + 50;
	};

  DAC_SetChannel1Data(DAC_Align_12b_R,2000);//初始值为0	
	
	//初始化值
	adcx=Get_Adc_Average(ADC_Channel_5,10);		//得到ADC转换值
	Line[2] = 30+m;Line[3] = 480-30-temp_1;
	while(1)
	{
			//DAC
			DAC_SetChannel1Data(DAC_Align_12b_R,4095 );//设置DAC值
 			dacx=DAC_GetDataOutputValue(DAC_Channel_1);//读取前面设置DAC的值
			LCD_ShowxNum(94,150,dacx,4,16,0);     	   //显示DAC寄存器值
			temp=(float)dacx*(3.3/4096);			         //得到DAC电压值
			dacx=temp;
 			LCD_ShowxNum(94,170,temp,1,16,0);     	   //显示电压值整数部分
 			temp-=dacx;
			temp*=1000;
			LCD_ShowxNum(110,170,temp,3,16,0X80); 	   //显示电压值的小数部分
			
			//ADC
			Line[0] = Line[2];
			Line[1] = Line[3];
 			adcx=Get_Adc_Average(ADC_Channel_5,10);		//得到ADC转换值
			temp_1=(float)adcx*(RANGE/4096);	
			
			LCD_ShowxNum(94,20,temp_1,4,16,0);     	  //显示电压值整数部分
			//LCD_ShowChar(30+m,480-30-temp_1-20,'.',24,1);	//用 . 显示
			
			Line[2] = 30+m;Line[3] = 480-30-temp_1;
			LCD_DrawLine(Line[0],Line[1],Line[2],Line[3]);		//用线显示
				
			temp=(float)adcx*(3.3/4096);			        //得到ADC电压值
			adcx=temp;
 			LCD_ShowxNum(94,190,temp,1,16,0);     	  //显示电压值整数部分
 			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(110,190,temp,3,16,0X80); 	  //显示电压值的小数部分
			LED0=!LED0;	
			
		  m=m+1; 
			if(m == 770)
			{
				LCD_Fill(31,0,800,449,	WHITE);
				adcx=Get_Adc_Average(ADC_Channel_5,10);		//得到ADC转换值
				temp_1=(float)adcx*(RANGE/4096);	
				m = 0;				
				Line[2] = 30+m;Line[3] = 480-30-temp_1;
			}
			delay_ms(1);	 
	}	
}

