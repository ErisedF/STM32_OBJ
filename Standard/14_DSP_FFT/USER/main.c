#include "sys.h"
#include "delay.h"  
#include "usart.h"  
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "timer.h" 
#include "math.h" 
#include "arm_math.h"  




#define FFT_LENGTH		1024 		//FFT���ȣ�Ĭ����1024��FFT

float fft_inputbuf[FFT_LENGTH*2];	//FFT��������
float fft_outputbuf[FFT_LENGTH];	//FFT�������

u8 timeout;//��ʱ���������


int main(void)
{ 
  arm_cfft_radix4_instance_f32 scfft;
 	u8 key,t=0;
	u16 i; 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	
	LED_Init();					//��ʼ��LED
	KEY_Init();					//��ʼ������
// 	LCD_Init();					//��ʼ��LCD
	
	arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1);//��ʼ��scfft�ṹ��&scfft  FFT_LENGTH FFT���� �� 0����Ҷ�任 ��1 �Ƿ�λȡ��
	
 	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			for(i=0;i<FFT_LENGTH;i++)		
			{
				//���ʵ��
				 fft_inputbuf[2*i]=100+
				                   10*arm_sin_f32(2 * PI * i / FFT_LENGTH)+				//1HZ		sin(2��*f*x) x = 1/1024
													 30*arm_sin_f32(2 * PI *i * 4 / FFT_LENGTH)+		//4HZ
				                   50*arm_cos_f32(2 * PI * i * 8 / FFT_LENGTH);		//8HZ				���������ź�ʵ��
				//����鲿
				 fft_inputbuf[2*i+1]=0;
			}
			
			arm_cfft_radix4_f32(&scfft,fft_inputbuf);									//��4 FFT����  ��fft_inputbufȡֵ������ٷ���fft_inputbuf 	
			arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);	//fft_inputbuf��ȡģ�����fft_outputbuf
			for(i=0;i<FFT_LENGTH;i++)
			{
				printf("%f\r\n",fft_outputbuf[i]);
			}
		}else delay_ms(10);
		//����״̬��ʾ
		t++;
		if((t%10)==0)LED0=!LED0;		  
	}
}
 
