#include "sys.h"
#include "delay.h"  
#include "usart.h"  
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "timer.h" 
#include "math.h" 
#include "arm_math.h"  




#define FFT_LENGTH		1024 		//FFT长度，默认是1024点FFT

float fft_inputbuf[FFT_LENGTH*2];	//FFT输入数组
float fft_outputbuf[FFT_LENGTH];	//FFT输出数组

u8 timeout;//定时器溢出次数


int main(void)
{ 
  arm_cfft_radix4_instance_f32 scfft;
 	u8 key,t=0;
	u16 i; 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	LED_Init();					//初始化LED
	KEY_Init();					//初始化按键
// 	LCD_Init();					//初始化LCD
	
	arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1);//初始化scfft结构体&scfft  FFT_LENGTH FFT长度 ； 0傅里叶变换 ；1 是否按位取反
	
 	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			for(i=0;i<FFT_LENGTH;i++)		
			{
				//存放实部
				 fft_inputbuf[2*i]=100+
				                   10*arm_sin_f32(2 * PI * i / FFT_LENGTH)+				//1HZ		sin(2Π*f*x) x = 1/1024
													 30*arm_sin_f32(2 * PI *i * 4 / FFT_LENGTH)+		//4HZ
				                   50*arm_cos_f32(2 * PI * i * 8 / FFT_LENGTH);		//8HZ				生成输入信号实部
				//存放虚部
				 fft_inputbuf[2*i+1]=0;
			}
			
			arm_cfft_radix4_f32(&scfft,fft_inputbuf);									//基4 FFT计算  从fft_inputbuf取值计算后再放入fft_inputbuf 	
			arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);	//fft_inputbuf求取模后放入fft_outputbuf
			for(i=0;i<FFT_LENGTH;i++)
			{
				printf("%f\r\n",fft_outputbuf[i]);
			}
		}else delay_ms(10);
		//运行状态提示
		t++;
		if((t%10)==0)LED0=!LED0;		  
	}
}
 
