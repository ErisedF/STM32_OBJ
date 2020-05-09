#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"



int main(void)
{ 
 	u16 adcx;
	float temp;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	LED_Init();					//初始化LED 
 	LCD_Init();         //初始化LCD接口
	Adc_Init();         //初始化ADC
	LCD_Display_Dir(1);								//设置屏幕显示方向

	POINT_COLOR=RED; 
	LCD_ShowString(30,130,200,16,16,"ADC1_CH5_VAL:");	      
	LCD_ShowString(30,150,200,16,16,"ADC1_CH5_VOL:0.000V");	//先在固定位置显示小数点      
	while(1)
	{ 
		adcx=Get_Adc_Average(ADC_Channel_5,10);//获取通道5的转换值，10次取平均
		LCD_ShowxNum(134,130,adcx,4,16,0);     //显示ADCC采样后的原始值
		temp=(float)adcx*(3.3/4096);           //基准电压为3.3v 
		adcx=temp;                             //取整数
		LCD_ShowxNum(134,150,adcx,1,16,0);     //显示电压值的整数部分
		temp-=adcx;                            //留下小数部分，比如3.1111-3=0.1111
		temp*=1000;                            //小数部分乘以1000，保留三位小数
		LCD_ShowxNum(150,150,temp,3,16,0X80);  //显示小数部分
		LED0=!LED0;
		delay_ms(50);	
	}
}



