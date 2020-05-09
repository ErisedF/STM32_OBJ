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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
 	LCD_Init();         //��ʼ��LCD�ӿ�
	Adc_Init();         //��ʼ��ADC
	LCD_Display_Dir(1);								//������Ļ��ʾ����

	POINT_COLOR=RED; 
	LCD_ShowString(30,130,200,16,16,"ADC1_CH5_VAL:");	      
	LCD_ShowString(30,150,200,16,16,"ADC1_CH5_VOL:0.000V");	//���ڹ̶�λ����ʾС����      
	while(1)
	{ 
		adcx=Get_Adc_Average(ADC_Channel_5,10);//��ȡͨ��5��ת��ֵ��10��ȡƽ��
		LCD_ShowxNum(134,130,adcx,4,16,0);     //��ʾADCC�������ԭʼֵ
		temp=(float)adcx*(3.3/4096);           //��׼��ѹΪ3.3v 
		adcx=temp;                             //ȡ����
		LCD_ShowxNum(134,150,adcx,1,16,0);     //��ʾ��ѹֵ����������
		temp-=adcx;                            //����С�����֣�����3.1111-3=0.1111
		temp*=1000;                            //С�����ֳ���1000��������λС��
		LCD_ShowxNum(150,150,temp,3,16,0X80);  //��ʾС������
		LED0=!LED0;
		delay_ms(50);	
	}
}



