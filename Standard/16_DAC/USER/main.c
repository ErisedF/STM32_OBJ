#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "dac.h"
#include "key.h"

//#define SSD_HOR_RESOLUTION		800		//LCDˮƽ�ֱ���
//#define SSD_VER_RESOLUTION		480		//LCD��ֱ�ֱ���
#define N 770
#define RANGE 330.0

//u16	ADC_buffer [N];
u16 Line [4] = {0,0,0,0};
int main(void)
{ 
	u16 dacx;
	u16 adcx;
	
	u16 i; 		//��������
	u16 y = 100; 		//��������
	u16 m = 0;

	float temp;
	float temp_1;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);      //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	
	LED_Init();					//��ʼ��LED 
 	LCD_Init();					//LCD��ʼ��
	Adc_Init(); 				//adc��ʼ��
	KEY_Init(); 				//������ʼ��
	Dac1_Init();		 		//DACͨ��1��ʼ��	
	
	
	LCD_Display_Dir(1);
	POINT_COLOR=BLUE;//��������Ϊ��ɫ  
	LCD_DrawLine(30,0,30,480);		//������
	LCD_DrawLine(0,450,800,450);	//������
	
	
	POINT_COLOR=RED;//����������ɫ
	for(i = 7 ; i > 0 ; i-- )
	{
		
		LCD_Draw_Circle(30,y,3);
		LCD_ShowString(8,y,200,16,16,".");	//���ڹ̶�λ����ʾС���� 
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

  DAC_SetChannel1Data(DAC_Align_12b_R,2000);//��ʼֵΪ0	
	
	//��ʼ��ֵ
	adcx=Get_Adc_Average(ADC_Channel_5,10);		//�õ�ADCת��ֵ
	Line[2] = 30+m;Line[3] = 480-30-temp_1;
	while(1)
	{
			//DAC
			DAC_SetChannel1Data(DAC_Align_12b_R,4095 );//����DACֵ
 			dacx=DAC_GetDataOutputValue(DAC_Channel_1);//��ȡǰ������DAC��ֵ
			LCD_ShowxNum(94,150,dacx,4,16,0);     	   //��ʾDAC�Ĵ���ֵ
			temp=(float)dacx*(3.3/4096);			         //�õ�DAC��ѹֵ
			dacx=temp;
 			LCD_ShowxNum(94,170,temp,1,16,0);     	   //��ʾ��ѹֵ��������
 			temp-=dacx;
			temp*=1000;
			LCD_ShowxNum(110,170,temp,3,16,0X80); 	   //��ʾ��ѹֵ��С������
			
			//ADC
			Line[0] = Line[2];
			Line[1] = Line[3];
 			adcx=Get_Adc_Average(ADC_Channel_5,10);		//�õ�ADCת��ֵ
			temp_1=(float)adcx*(RANGE/4096);	
			
			LCD_ShowxNum(94,20,temp_1,4,16,0);     	  //��ʾ��ѹֵ��������
			//LCD_ShowChar(30+m,480-30-temp_1-20,'.',24,1);	//�� . ��ʾ
			
			Line[2] = 30+m;Line[3] = 480-30-temp_1;
			LCD_DrawLine(Line[0],Line[1],Line[2],Line[3]);		//������ʾ
				
			temp=(float)adcx*(3.3/4096);			        //�õ�ADC��ѹֵ
			adcx=temp;
 			LCD_ShowxNum(94,190,temp,1,16,0);     	  //��ʾ��ѹֵ��������
 			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(110,190,temp,3,16,0X80); 	  //��ʾ��ѹֵ��С������
			LED0=!LED0;	
			
		  m=m+1; 
			if(m == 770)
			{
				LCD_Fill(31,0,800,449,	WHITE);
				adcx=Get_Adc_Average(ADC_Channel_5,10);		//�õ�ADCת��ֵ
				temp_1=(float)adcx*(RANGE/4096);	
				m = 0;				
				Line[2] = 30+m;Line[3] = 480-30-temp_1;
			}
			delay_ms(1);	 
	}	
}

