#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 



//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart1_send_char(u8 c)
{

	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,c);   

} 
//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0X88;	//֡ͷ
	send_buf[1]=fun;	//������
	send_buf[2]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//��������
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//����У���	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//�������ݵ�����1 
}
//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;		//��λ
	tbuf[1]=aacx&0XFF;				//��λ
	
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	
	usart1_niming_report(0XA1,tbuf,12);//�Զ���֡,0XA1
}	
//ͨ������1�ϱ���������̬���ݸ�����
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)
		tbuf[i]=0;//��0
	
	tbuf[0]=(aacx>>8)&0XFF;		//��8λ
	tbuf[1]=aacx&0XFF;				//��8λ
	
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	
	usart1_niming_report(0XAF,tbuf,28);//�ɿ���ʾ֡,0XAF
} 
 

///******************************************************************
//�������˲� 1 
//��DMP������ĸ����ǽ���ƽ���˲�
//******************************************************************/
//	float Q = 0.01;			//ϵͳ��������Э������ Խ�󣬶�ģ�͵�����ԽС
//	float R = 5;				//������������Э������ Խ�󣬶Թ۲���������Խ��
//	
//	float A = 1;				//һ��ת�ƾ���
////	float B = 1;			//���ƾ���
//	float H = 1;				//��������

//	
//	float X_0 = 0;			//��ǰʱ�̹���ֵ
//	float X_10;					//��һ�Ǹ�Ԥ��ֵ
//	float P_0 = 0;			//��ǰ���ƾ������
//	float P_10;					//Ԥ��������
//	float K_0;					//��ǰ����������

//float Kalman_Filter(float pitch)
//{
///*********** Ԥ��	************/	
//	X_10 = A * X_0;
//	P_10 = A * P_0 / A + Q;
//	
///*********** ����	************/
//	K_0 = (P_10 * H)/(H * P_10 * H + R );
//	X_0 = X_10 + K_0 * (pitch -H * X_10);
//	P_0 = (1 - K_0 * H) * P_10;
//	

//	return X_0;
//}


/******************************************************************
�������˲� 2
******************************************************************/
	float Q_angle_X = 0.001;		//������ ���ٶ�����Э����
	float Q_gyro_X = 0.001;			//������ Ư������Э����
	float Q_bias_X = 0.001;			//�����ǽ��ٶ����Ư��Э�����
	float R_angle_X = 0.1;			//�����ǽ��ٶȾ������
	float dt = 0.0519;					//�˲�������ʱ��
	
	float X_angle_0 = 0;				//��ǰ�Ƕȹ���ֵ
	float X_angle_10 = 0;				//��һ��ʱ̬Ԥ��ֵ
	
	float X_PAL_0 = 0;					//��ǰ���ٶȹ���ֵ
	float X_PAL_10 = 0;					//��һʱ�̽��ٶ�Ԥ��ֵ
	
	float	err;									//Z-HX
	//����������
	float X_K_0[2][1] = {
												0,
												0
											};	
	//������һ��Ԥ�����
	float A[2][2] = {
											{1,1},
											{0,1}
									};
	//���ƾ������
	float P_0[2][2] = {
											{1,1},
											{1,1}

											};
	//Ԥ��������
	float P_10[2][2] = {
											{0,0},
											{0,0}
										};
	float P_mid[4] = {0,0,0,0};		//����м�ֵ AP��ֵ
	
//	float Q[2][2] = {
//											{Q_angle_X,0},
//											{0,Q_bias_X}
//										};

	float Kalman_Filter(float pitch,u16 GYRO_X)
	{
		
		X_PAL_0 = GYRO_X / 16.4;				//���Ծ��� ��þ�����ٶ�
/******************************************************************
										Ԥ��								
		״̬����:
״̬���������  ���ٶ�
�۲����������
				|1  dt	|						|Q_angle_X		0					|
		A = |				|			Q = 	|												|
				|0		1 |  					|0						Q_bias_X	|
		
		
		H = | 1		0|
******************************************************************/
		X_angle_10 = X_angle_0 + X_PAL_0 * dt;			//״̬����X_angle
		X_PAL_10 = X_PAL_0;													//״̬����X_PAL
		
		P_mid[0] = A[0][0] * P_0[0][0] + P_0[1][0] * dt;
		P_mid[1] = A[0][0] * P_0[0][1] + P_0[1][1] * dt;
		P_mid[2] = P_0[1][0];
		P_mid[3] = P_0[1][1];
		
		P_10[0][0] = P_mid[0] + Q_angle_X;
		P_10[0][1] = (P_mid[0] * (-dt)) + P_mid[1] + 0;
		P_10[1][0] = P_mid[2] + 0;
		P_10[1][1] = (P_mid[0] * (-dt)) + P_mid[3] + Q_bias_X;
		
/******************************************************************
										����								
******************************************************************/
		X_K_0[0][0] = P_10[0][0] / (P_10[0][0] + R_angle_X);
		X_K_0[1][0] = P_10[1][0] / (P_10[0][0] + R_angle_X);
		
		err = pitch - X_angle_10;
		X_angle_0 = X_angle_10 + X_K_0[0][0] * err;
		X_PAL_0 	= X_PAL_10 + X_K_0[1][0] * err;
		
		P_0[0][0] = P_10[0][0] * (1 - X_K_0[0][0]) + P_10[1][0];
		P_0[0][1] = P_10[0][1] * (1 - X_K_0[0][0]) + P_10[1][1];
		P_0[1][0] = P_10[0][0] * (1 - X_K_0[1][0]) + P_10[1][0];
		P_0[1][1] = P_10[0][1] * (1 - X_K_0[0][0]) + P_10[1][1];
		
		return X_angle_0;
	}

/******************************************************************
������
******************************************************************/
int main(void)
{ 
	float KF;
	//short temp_1;
	//const char KF_1;
	//char KF_str[30];
	
	u8 t=0,report=1;			//Ĭ�Ͽ����ϱ�
	u8 key;
	float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;			//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	short temp;								//�¶�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  		//��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ500000
	LED_Init();						//��ʼ��LED 
	KEY_Init();						//��ʼ������
 	LCD_Init();						//LCD��ʼ��
	MPU_Init();						//��ʼ��MPU6050

	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"MPU6050 TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2014/5/9");

	
	while(mpu_dmp_init())
	{
		LCD_ShowString(30,130,200,16,16,"MPU6050 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}
	LCD_ShowString(30,130,200,16,16,"MPU6050 OK");
	LCD_ShowString(30,150,200,16,16,"KEY0:UPLOAD ON/OFF");
	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
 	LCD_ShowString(30,170,200,16,16,"UPLOAD ON ");	 
 	LCD_ShowString(30,200,200,16,16," Temp:    . C");	
 	LCD_ShowString(30,220,200,16,16,"Pitch:    . C");	
 	LCD_ShowString(30,240,200,16,16," Roll:    . C");	 
 	LCD_ShowString(30,260,200,16,16," Yaw :    . C");	 
 	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			report=!report;
			if(report)LCD_ShowString(30,170,200,16,16,"UPLOAD ON ");
			else LCD_ShowString(30,170,200,16,16,"UPLOAD OFF");
		}
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)		//��ȡDMP�н�����ֵ
		{ 
			temp=MPU_Get_Temperature();	//�õ��¶�ֵ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			if(report)
				mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//���Զ���֡���ͼ��ٶȺ�������ԭʼ����
			if(report)
				usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
			if((t%3)==0)
			{ 
				if(temp<0)
				{
					LCD_ShowChar(30+48,200,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else 
					LCD_ShowChar(30+48,200,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,200,temp/100,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,200,temp%10,1,16);		//��ʾС������ 
/***********	������	************/				
				temp=pitch*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,220,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else 
					LCD_ShowChar(30+48,220,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,220,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,220,temp%10,1,16);		//��ʾС������ 
/***********	�����	************/				
				temp=roll*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,240,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else 
					LCD_ShowChar(30+48,240,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,240,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,240,temp%10,1,16);		//��ʾС������ 
/***********	�����	************/				
				temp=yaw*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,260,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else 
					LCD_ShowChar(30+48,260,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,260,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,260,temp%10,1,16);		//��ʾС������  
				t=0;
				LED0=!LED0;//LED��˸
/***********	ԭʼ����	************/		
				LCD_ShowString(30,280,200,16,16,"aacx :");	 
				LCD_ShowString(30,300,200,16,16,"accy :");	 
				LCD_ShowString(30,320,200,16,16,"accz :");	 
				LCD_ShowNum(30+48+15,280,aacx/16384,5,16);		//��ʾ��������	   
				LCD_ShowNum(30+48+15,300,aacy/16384,5,16);		//��ʾ��������	   
				LCD_ShowNum(30+48+15,320,aacz/16384,5,16);		//��ʾ��������	   


				LCD_ShowString(30,350,200,16,16,"gyrox :");	 
				LCD_ShowString(30,370,200,16,16,"gyroy :");	 
				LCD_ShowString(30,390,200,16,16,"gyroz :");	 
				LCD_ShowNum(30+48+15,350,gyrox,5,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+15,370,gyroy,5,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+15,390,gyroz,5,16);		//��ʾ��������
				
				LCD_ShowString(30,420,200,16,16,"pitch :");	 
				LCD_ShowString(30,440,200,16,16,"roll :");	 
				LCD_ShowString(30,460,200,16,16,"yaw :");	 
				LCD_ShowNum(30+48+15,420,pitch*10,5,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+15,440,roll*10,5,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+15,460,yaw*10,5,16);		//��ʾ��������


//				KF = Kalman_Filter(pitch);				//ƽ���˲�
					KF = Kalman_Filter(pitch,gyrox);	//

				LCD_ShowString(30,480,200,16,16,"KF :");	 
				LCD_ShowNum(30+48+15,480,KF*10,5,16);		//��ʾ��������

				printf("KF:%f\n,PC:%f\r\n",KF,pitch);


			}
		}
		t++; 
	} 	
}

