#include "myiic.h"
#include "delay.h"

/******************************************************************
��ʼ��IIC
******************************************************************/
void IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;						//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;						//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);									//��ʼ��
	
	IIC_SCL=1;
	IIC_SDA=1;
}

/******************************************************************
����IIC��ʼ�ź�
IIC SDA SCL ͬʱΪ��ʱΪ����״̬
��SCLΪ��ʱ��SDA��͵�ƽ����
SDAΪ���
******************************************************************/
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;		//��SCLΪ��ʱ��SDA��͵�ƽ����
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  

/******************************************************************
����IICֹͣ�ź�
��SCLΪ��ʱ��SDA��ߵ�ƽ����
SDAΪ���
******************************************************************/
void IIC_Stop(void)
{
	SDA_OUT();		//sda�����
	IIC_SCL=0;
	IIC_SDA=0;		//��SCLΪ��ʱ��SDA��ߵ�ƽ����
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;		//����I2C���߽����ź�
	delay_us(4);							   	
}

/******************************************************************
�ȴ�Ӧ���źŵ���
����ֵ��1������Ӧ��ʧ��
        0������Ӧ��ɹ�

SDA����
******************************************************************/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;					 //����ʱ��
	SDA_IN();     					 //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 

/******************************************************************
����ACKӦ��
SDA���
******************************************************************/
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;			//0 ���ܳɹ�
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

/******************************************************************
������ACKӦ��
SDA���
******************************************************************/
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;			//1 ����ʧ��
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     


/******************************************************************
IIC����һ���ֽ�
���شӻ�����Ӧ��
SDA ���
SDA������SCL�ߵ�ƽ�ڼ���Ч
******************************************************************/
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA_OUT(); 	    
    IIC_SCL=0;				//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {			
			IIC_SDA=(txd&0x80)>>7;		//ȡ���λ ��λ�����λ
			txd<<=1; 	  							//�Ѵθ�λ��Ϊ���λ
			delay_us(2);   
			IIC_SCL=1;
			delay_us(2); 
			IIC_SCL=0;	
			delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
/******************************************************************
IIC��ȡһ���ֽ�
���شӻ�����Ӧ��
SDA ���
SDA������SCL�ߵ�ƽ�ڼ���Ч
******************************************************************/
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
	
  for(i=0;i<8;i++ )
	{
		IIC_SCL=0; 
		delay_us(2);
		IIC_SCL=1;
		receive<<=1;
		if(READ_SDA)
			receive++;   
		delay_us(1); 
  }					 
		if (!ack)
			IIC_NAck();//����nACK
		else
			IIC_Ack(); //����ACK   
		return receive;
}



























