#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 



//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{

	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,c);   

} 
//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X88;	//帧头
	send_buf[1]=fun;	//功能字
	send_buf[2]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//复制数据
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}
//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;		//高位
	tbuf[1]=aacx&0XFF;				//低位
	
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
	
	usart1_niming_report(0XA1,tbuf,12);//自定义帧,0XA1
}	
//通过串口1上报结算后的姿态数据给电脑
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)
		tbuf[i]=0;//清0
	
	tbuf[0]=(aacx>>8)&0XFF;		//高8位
	tbuf[1]=aacx&0XFF;				//低8位
	
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
	
	usart1_niming_report(0XAF,tbuf,28);//飞控显示帧,0XAF
} 
 

///******************************************************************
//卡尔曼滤波 1 
//对DMP解算出的俯仰角进行平滑滤波
//******************************************************************/
//	float Q = 0.01;			//系统噪声序列协方差阵 越大，对模型的信任越小
//	float R = 5;				//测量噪声序列协方差阵 越大，对观测数据信任越低
//	
//	float A = 1;				//一步转移矩阵
////	float B = 1;			//控制矩阵
//	float H = 1;				//测量矩阵

//	
//	float X_0 = 0;			//当前时刻估计值
//	float X_10;					//下一是个预测值
//	float P_0 = 0;			//当前估计均方误差
//	float P_10;					//预测均方误差
//	float K_0;					//当前卡尔曼增益

//float Kalman_Filter(float pitch)
//{
///*********** 预测	************/	
//	X_10 = A * X_0;
//	P_10 = A * P_0 / A + Q;
//	
///*********** 更新	************/
//	K_0 = (P_10 * H)/(H * P_10 * H + R );
//	X_0 = X_10 + K_0 * (pitch -H * X_10);
//	P_0 = (1 - K_0 * H) * P_10;
//	

//	return X_0;
//}


/******************************************************************
卡尔曼滤波 2
******************************************************************/
	float Q_angle_X = 0.001;		//陀螺仪 角速度噪声协方差
	float Q_gyro_X = 0.001;			//陀螺仪 漂移噪声协方差
	float Q_bias_X = 0.001;			//陀螺仪角速度零点漂移协方误差
	float R_angle_X = 0.1;			//陀螺仪角速度均方误差
	float dt = 0.0519;					//滤波器采样时间
	
	float X_angle_0 = 0;				//当前角度估计值
	float X_angle_10 = 0;				//下一个时态预测值
	
	float X_PAL_0 = 0;					//当前角速度估计值
	float X_PAL_10 = 0;					//下一时刻角速度预测值
	
	float	err;									//Z-HX
	//卡尔曼增益
	float X_K_0[2][1] = {
												0,
												0
											};	
	//卡尔曼一步预测矩阵
	float A[2][2] = {
											{1,1},
											{0,1}
									};
	//估计均方误差
	float P_0[2][2] = {
											{1,1},
											{1,1}

											};
	//预测均方误差
	float P_10[2][2] = {
											{0,0},
											{0,0}
										};
	float P_mid[4] = {0,0,0,0};		//存放中间值 AP的值
	
//	float Q[2][2] = {
//											{Q_angle_X,0},
//											{0,Q_bias_X}
//										};

	float Kalman_Filter(float pitch,u16 GYRO_X)
	{
		
		X_PAL_0 = GYRO_X / 16.4;				//除以精度 获得具体角速度
/******************************************************************
										预测								
		状态方程:
状态量：横滚角  角速度
观测量：横滚角
				|1  dt	|						|Q_angle_X		0					|
		A = |				|			Q = 	|												|
				|0		1 |  					|0						Q_bias_X	|
		
		
		H = | 1		0|
******************************************************************/
		X_angle_10 = X_angle_0 + X_PAL_0 * dt;			//状态变量X_angle
		X_PAL_10 = X_PAL_0;													//状态变量X_PAL
		
		P_mid[0] = A[0][0] * P_0[0][0] + P_0[1][0] * dt;
		P_mid[1] = A[0][0] * P_0[0][1] + P_0[1][1] * dt;
		P_mid[2] = P_0[1][0];
		P_mid[3] = P_0[1][1];
		
		P_10[0][0] = P_mid[0] + Q_angle_X;
		P_10[0][1] = (P_mid[0] * (-dt)) + P_mid[1] + 0;
		P_10[1][0] = P_mid[2] + 0;
		P_10[1][1] = (P_mid[0] * (-dt)) + P_mid[3] + Q_bias_X;
		
/******************************************************************
										更新								
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
主函数
******************************************************************/
int main(void)
{ 
	float KF;
	//short temp_1;
	//const char KF_1;
	//char KF_str[30];
	
	u8 t=0,report=1;			//默认开启上报
	u8 key;
	float pitch,roll,yaw; 		//欧拉角
	short aacx,aacy,aacz;			//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	short temp;								//温度
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  		//初始化延时函数
	uart_init(115200);		//初始化串口波特率为500000
	LED_Init();						//初始化LED 
	KEY_Init();						//初始化按键
 	LCD_Init();						//LCD初始化
	MPU_Init();						//初始化MPU6050

	POINT_COLOR=RED;//设置字体为红色 
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
	POINT_COLOR=BLUE;//设置字体为蓝色 
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
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)		//获取DMP中解算后的值
		{ 
			temp=MPU_Get_Temperature();	//得到温度值
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
			if(report)
				mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//用自定义帧发送加速度和陀螺仪原始数据
			if(report)
				usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
			if((t%3)==0)
			{ 
				if(temp<0)
				{
					LCD_ShowChar(30+48,200,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}else 
					LCD_ShowChar(30+48,200,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,200,temp/100,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,200,temp%10,1,16);		//显示小数部分 
/***********	俯仰角	************/				
				temp=pitch*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,220,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}else 
					LCD_ShowChar(30+48,220,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,220,temp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,220,temp%10,1,16);		//显示小数部分 
/***********	横滚角	************/				
				temp=roll*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,240,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}else 
					LCD_ShowChar(30+48,240,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,240,temp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,240,temp%10,1,16);		//显示小数部分 
/***********	航向角	************/				
				temp=yaw*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,260,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}else 
					LCD_ShowChar(30+48,260,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,260,temp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,260,temp%10,1,16);		//显示小数部分  
				t=0;
				LED0=!LED0;//LED闪烁
/***********	原始数据	************/		
				LCD_ShowString(30,280,200,16,16,"aacx :");	 
				LCD_ShowString(30,300,200,16,16,"accy :");	 
				LCD_ShowString(30,320,200,16,16,"accz :");	 
				LCD_ShowNum(30+48+15,280,aacx/16384,5,16);		//显示整数部分	   
				LCD_ShowNum(30+48+15,300,aacy/16384,5,16);		//显示整数部分	   
				LCD_ShowNum(30+48+15,320,aacz/16384,5,16);		//显示整数部分	   


				LCD_ShowString(30,350,200,16,16,"gyrox :");	 
				LCD_ShowString(30,370,200,16,16,"gyroy :");	 
				LCD_ShowString(30,390,200,16,16,"gyroz :");	 
				LCD_ShowNum(30+48+15,350,gyrox,5,16);		//显示整数部分	    
				LCD_ShowNum(30+48+15,370,gyroy,5,16);		//显示整数部分	    
				LCD_ShowNum(30+48+15,390,gyroz,5,16);		//显示整数部分
				
				LCD_ShowString(30,420,200,16,16,"pitch :");	 
				LCD_ShowString(30,440,200,16,16,"roll :");	 
				LCD_ShowString(30,460,200,16,16,"yaw :");	 
				LCD_ShowNum(30+48+15,420,pitch*10,5,16);		//显示整数部分	    
				LCD_ShowNum(30+48+15,440,roll*10,5,16);		//显示整数部分	    
				LCD_ShowNum(30+48+15,460,yaw*10,5,16);		//显示整数部分


//				KF = Kalman_Filter(pitch);				//平滑滤波
					KF = Kalman_Filter(pitch,gyrox);	//

				LCD_ShowString(30,480,200,16,16,"KF :");	 
				LCD_ShowNum(30+48+15,480,KF*10,5,16);		//显示整数部分

				printf("KF:%f\n,PC:%f\r\n",KF,pitch);


			}
		}
		t++; 
	} 	
}

