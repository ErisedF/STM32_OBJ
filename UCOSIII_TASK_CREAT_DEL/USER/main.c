#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "cap_key.h"
#include "includes.h"
#include "os_app_hooks.h"

//ALIENTEK ̽����STM32F407������ UCOSIIIʵ��
//��4-1 UCOSIII UCOSIII��ֲ

//UCOSIII���������ȼ��û�������ʹ��
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()


/******************************************************************
��ʼ������
******************************************************************/
#define 	START_TASK_PRIO				3						//�������ȼ�
#define		START_STK_SIZE				128					//�����ջ��С128*4byte
OS_TCB		StartTaskTCB;											//������ƿ�
CPU_STK		START_TASK_STK[START_STK_SIZE];		//�����ջ
void start_task(void *p_arg);								//������
/******************************************************************
����1����
******************************************************************/
#define 	TASK1_TASK_PRIO				4						//�������ȼ�
#define		TASK1_STK_SIZE				128					//�����ջ��С128*4byte
OS_TCB		TASK1_TaskTCB;											//������ƿ�
CPU_STK		TASK1_TASK_STK[TASK1_STK_SIZE];		//�����ջ
void task1_task(void *p_arg);								//������
/******************************************************************
����2����
******************************************************************/
#define 	TASK2_TASK_PRIO				5						//�������ȼ�
#define		TASK2_STK_SIZE				128					//�����ջ��С128*4byte
OS_TCB		TASK2_TaskTCB;											//������ƿ�
CPU_STK		TASK2_TASK_STK[TASK2_STK_SIZE];		//�����ջ
void task2_task(void *p_arg);								//������
/******************************************************************
����3����
******************************************************************/
#define 	TASK3_TASK_PRIO				6						//�������ȼ�
#define		TASK3_STK_SIZE				128					//�����ջ��С128*4byte
OS_TCB		TASK3_TaskTCB;											//������ƿ�
CPU_STK		TASK3_TASK_STK[TASK3_STK_SIZE];		//�����ջ
void task3_task(void *p_arg);								//������
/******************************************************************
����4����
******************************************************************/
#define 	TASK4_TASK_PRIO				7						//�������ȼ�
#define		TASK4_STK_SIZE				128					//�����ջ��С128*4byte
OS_TCB		TASK4_TaskTCB;											//������ƿ�
CPU_STK		TASK4_TASK_STK[TASK4_STK_SIZE];		//�����ջ
void task4_task(void *p_arg);								//������
/******************************************************************
����5����
******************************************************************/
#define 	TASK5_TASK_PRIO				8						//�������ȼ�
#define		TASK5_STK_SIZE				128					//�����ջ��С128*4byte
OS_TCB		TASK5_TaskTCB;											//������ƿ�
CPU_STK		TASK5_TASK_STK[TASK5_STK_SIZE];		//�����ջ
void task5_task(void *p_arg);								//������
/******************************************************************
����6����
******************************************************************/
#define 	TASK6_TASK_PRIO				9						//�������ȼ�
#define		TASK6_STK_SIZE				128					//�����ջ��С128*4byte
OS_TCB		TASK6_TaskTCB;											//������ƿ�
CPU_STK		TASK6_TASK_STK[TASK6_STK_SIZE];		//�����ջ
void task6_task(void *p_arg);								//������
/******************************************************************
����7����
******************************************************************/
#define 	TASK7_TASK_PRIO				10						//�������ȼ�
#define		TASK7_STK_SIZE				128					//�����ջ��С128*4byte
OS_TCB		TASK7_TaskTCB;											//������ƿ�
CPU_STK		TASK7_TASK_STK[TASK7_STK_SIZE];		//�����ջ
void task7_task(void *p_arg);								//������


/******************************************************************
���嶨ʱ��
******************************************************************/
OS_TMR 	tmr1;		//��ʱ��1�ṹ��
OS_TMR	tmr2;		//��ʱ��2�ṹ��
void tmr1_callback(void *p_tmr, void *p_arg); 	//��ʱ��1�ص�����
void tmr2_callback(void *p_tmr, void *p_arg);		//��ʱ��2�ص�����


/******************************************************************
�����ź���
******************************************************************/
OS_SEM	MY_SEM;		//����һ���ź��������ڷ��ʹ�����Դ


/******************************************************************
������Ϣ����
******************************************************************/
#define KEYMSG_Q_NUM	1	//������Ϣ���е�����
OS_Q KEY_Msg;						//����һ����Ϣ����

/******************************************************************
����ˢ����ɫ
******************************************************************/
int lcd_discolor[14]={	WHITE, BLACK, BLUE,  BRED,      
						GRED,  GBLUE, RED,   MAGENTA,       	 
						GREEN, CYAN,  YELLOW,BROWN, 			
						BRRED, GRAY };


int main(void)
{
	OS_ERR err;																						//����������
	CPU_SR_ALLOC();																				//�ٽ������
	delay_init(168);																			//��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//�жϷ���
	uart_init(115200);																		//USART��ʼ��
	LED_Init();																						//LED��ʼ��
	LCD_Init();																						//LCD��ʼ��
	KEY_Init();																						//������ʼ��
	TPAD_Init();																					//���ݰ�����ʼ��

/******************************************************************
LCD��ʾ����
******************************************************************/
	POINT_COLOR = RED;
	LCD_ShowString(100,20,266,24,24,"UCOSIII_TASK_CREAT_DEL");		
/******************************************************************
���ٽ����д�������
******************************************************************/
	OSInit(&err);				//UCOSIII��ʼ��
	OS_CRITICAL_ENTER();	//�����ٽ���
	OSTaskCreate (
										(OS_TCB        *)&StartTaskTCB,				//������ƿ�
                    (CPU_CHAR      *)"start task",				//��������
                    (OS_TASK_PTR    )start_task,					//������
                    (void          *)0,										//���������ݲ���
                    (OS_PRIO        )START_TASK_PRIO,			//�������ȼ�
                    (CPU_STK       *)&START_TASK_STK[0],	//�����ջ����ַ ��0��ʼ
                    (CPU_STK_SIZE   )START_STK_SIZE/10,		//�����ջջ������ջ��С/10
                    (CPU_STK_SIZE   )START_STK_SIZE,			//�����ջ��С
                    (OS_MSG_QTY     )0,										//�����ڽ���Ϣ���д�С��
                    (OS_TICK        )0,										//ʱ��Ƭ��ת����ʱ��
                    (void          *)0,										//�û�����洢��
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//����ѡ��������
                    (OS_ERR        *)&err
									);
	OS_CRITICAL_EXIT();		//�˳��ٽ���
	OSStart(&err);
}

/******************************************************************
��ʼ������
�ڿ�ʼ�������д����µ�������
******************************************************************/
void start_task(void *p_arg)
{
	OS_ERR err;											//����������
	CPU_SR_ALLOC();									//�ٽ������
	p_arg = p_arg;									//ȥ������δʹ�þ���
	CPU_Init();											//CPU��ʼ��
	
	
/******************************************************************
������������
******************************************************************/
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN			//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  		//��ʹ��ʱ��Ƭ��ת��ʱ��
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
#endif		
	

	OS_CRITICAL_ENTER();	//�����ٽ���
/******************************************************************
���ٽ����д����ź���
******************************************************************/
	OSSemCreate (
										(OS_SEM      *)&MY_SEM,				//ָ���ź��������ƿ�
										(CPU_CHAR    *)"MY_SEM",			//�ź�������
										(OS_SEM_CTR   )0,							//�ź�����ʼֵ
										(OS_ERR      *)&err						//�������
									 );
/******************************************************************
���ٽ����д�����Ϣ����
******************************************************************/
	OSQCreate (
										(OS_Q					*)&KEY_Msg,				//��Ϣ����
										(CPU_CHAR	 	  *)"KEY Msg",			//��Ϣ��������
										(OS_MSG_QTY		 )KEYMSG_Q_NUM,		//��Ϣ���г��ȣ���������Ϊ1
										(OS_ERR		  	*)&err 						//������
						);		


/******************************************************************
���ٽ����д�����ʱ��1����ʱ��2
******************************************************************/
	//��ʱ��1
	OSTmrCreate (
									 (OS_TMR               *)&tmr1,									//ָ����ʱ��
                   (CPU_CHAR             *)"tmr1",								//��ʱ������
                   (OS_TICK               )20,										//��ʼ��ʱʱ�� 20*10 = 200ms
                   (OS_TICK               )100,										//����ģʽ��ʱʱ�� 100*10 = 1000ms
                   (OS_OPT                )OS_OPT_TMR_PERIODIC,		//��ʱ��ģʽ������ģʽ
                   (OS_TMR_CALLBACK_PTR   )tmr1_callback,					//�ص�����
                   (void                 *)0,											//����Ϊ0
                   (OS_ERR               *)&err										//���صĴ�����
							);
	//��ʱ��2
	OSTmrCreate (
									 (OS_TMR               *)&tmr2,									//ָ����ʱ��
                   (CPU_CHAR             *)"tmr2",								//��ʱ������
                   (OS_TICK               )200,										//��ʼ��ʱʱ�� 200*10 = 2000ms
                   (OS_TICK               )0,											//����ģʽ��ʱʱ�� 100*100 = 10000ms
                   (OS_OPT                )OS_OPT_TMR_ONE_SHOT,		//��ʱ��ģʽ������ģʽ
                   (OS_TMR_CALLBACK_PTR   )tmr2_callback,					//�ص�����
                   (void                 *)0,											//����Ϊ0
                   (OS_ERR               *)&err										//���صĴ�����
							);

/******************************************************************
���ٽ����д�������1������2������3
******************************************************************/
	//����1
	OSTaskCreate (
										(OS_TCB        *)&TASK1_TaskTCB,			//������ƿ�
                    (CPU_CHAR      *)"task1 task",				//��������
                    (OS_TASK_PTR    )task1_task,					//������
                    (void          *)0,										//���������ݲ���
                    (OS_PRIO        )TASK1_TASK_PRIO,			//�������ȼ�
                    (CPU_STK       *)&TASK1_TASK_STK[0],	//�����ջ����ַ ��0��ʼ
                    (CPU_STK_SIZE   )TASK1_STK_SIZE/10,		//�����ջջ������ջ��С/10
                    (CPU_STK_SIZE   )TASK1_STK_SIZE,			//�����ջ��С
                    (OS_MSG_QTY     )0,										//�����ڽ���Ϣ���д�С��
                    (OS_TICK        )0,						//0*5ms	//ʱ��Ƭ��ת����ʱ��
                    (void          *)0,										//�û�����洢��
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//����ѡ��������
                    (OS_ERR        *)&err
									);
	//����2
		OSTaskCreate (
										(OS_TCB        *)&TASK2_TaskTCB,			//������ƿ�
                    (CPU_CHAR      *)"task2 task",				//��������
                    (OS_TASK_PTR    )task2_task,					//������
                    (void          *)0,										//���������ݲ���
                    (OS_PRIO        )TASK2_TASK_PRIO,			//�������ȼ�
                    (CPU_STK       *)&TASK2_TASK_STK[0],	//�����ջ����ַ ��0��ʼ
                    (CPU_STK_SIZE   )TASK2_STK_SIZE/10,		//�����ջջ������ջ��С/10
                    (CPU_STK_SIZE   )TASK2_STK_SIZE,			//�����ջ��С
                    (OS_MSG_QTY     )0,										//�����ڽ���Ϣ���д�С��
                    (OS_TICK        )0,					//3*5ms		//ʱ��Ƭ��ת����ʱ��
                    (void          *)0,										//�û�����洢��
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//����ѡ��������
                    (OS_ERR        *)&err
									);	
	//����3
		OSTaskCreate (
										(OS_TCB        *)&TASK3_TaskTCB,			//������ƿ�
                    (CPU_CHAR      *)"task3 task",				//��������
                    (OS_TASK_PTR    )task3_task,					//������
                    (void          *)0,										//���������ݲ���
                    (OS_PRIO        )TASK3_TASK_PRIO,			//�������ȼ�
                    (CPU_STK       *)&TASK3_TASK_STK[0],	//�����ջ����ַ ��0��ʼ
                    (CPU_STK_SIZE   )TASK3_STK_SIZE/10,		//�����ջջ������ջ��С/10
                    (CPU_STK_SIZE   )TASK3_STK_SIZE,			//�����ջ��С
                    (OS_MSG_QTY     )0,										//�����ڽ���Ϣ���д�С��
                    (OS_TICK        )0,					//3*5ms		//ʱ��Ƭ��ת����ʱ��
                    (void          *)0,										//�û�����洢��
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//����ѡ��������
                    (OS_ERR        *)&err
									);							
	//����4
		OSTaskCreate (
										(OS_TCB        *)&TASK4_TaskTCB,			//������ƿ�
                    (CPU_CHAR      *)"task4 task",				//��������
                    (OS_TASK_PTR    )task4_task,					//������
                    (void          *)0,										//���������ݲ���
                    (OS_PRIO        )TASK4_TASK_PRIO,			//�������ȼ�
                    (CPU_STK       *)&TASK4_TASK_STK[0],	//�����ջ����ַ ��0��ʼ
                    (CPU_STK_SIZE   )TASK4_STK_SIZE/10,		//�����ջջ������ջ��С/10
                    (CPU_STK_SIZE   )TASK4_STK_SIZE,			//�����ջ��С
                    (OS_MSG_QTY     )0,										//�����ڽ���Ϣ���д�С��
                    (OS_TICK        )0,					//3*5ms		//ʱ��Ƭ��ת����ʱ��
                    (void          *)0,										//�û�����洢��
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//����ѡ��������
                    (OS_ERR        *)&err
									);
	//����5
		OSTaskCreate (
										(OS_TCB        *)&TASK5_TaskTCB,			//������ƿ�
                    (CPU_CHAR      *)"task5 task",				//��������
                    (OS_TASK_PTR    )task5_task,					//������
                    (void          *)0,										//���������ݲ���
                    (OS_PRIO        )TASK5_TASK_PRIO,			//�������ȼ�
                    (CPU_STK       *)&TASK5_TASK_STK[0],	//�����ջ����ַ ��0��ʼ
                    (CPU_STK_SIZE   )TASK5_STK_SIZE/10,		//�����ջջ������ջ��С/10
                    (CPU_STK_SIZE   )TASK5_STK_SIZE,			//�����ջ��С
                    (OS_MSG_QTY     )0,										//�����ڽ���Ϣ���д�С��
                    (OS_TICK        )0,					//3*5ms		//ʱ��Ƭ��ת����ʱ��
                    (void          *)0,										//�û�����洢��
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//����ѡ��������
                    (OS_ERR        *)&err
									);							
	//����6
		OSTaskCreate (
										(OS_TCB        *)&TASK6_TaskTCB,			//������ƿ�
                    (CPU_CHAR      *)"task6 task",				//��������
                    (OS_TASK_PTR    )task6_task,					//������
                    (void          *)0,										//���������ݲ���
                    (OS_PRIO        )TASK6_TASK_PRIO,			//�������ȼ�
                    (CPU_STK       *)&TASK6_TASK_STK[0],	//�����ջ����ַ ��0��ʼ
                    (CPU_STK_SIZE   )TASK6_STK_SIZE/10,		//�����ջջ������ջ��С/10
                    (CPU_STK_SIZE   )TASK6_STK_SIZE,			//�����ջ��С
                    (OS_MSG_QTY     )0,										//�����ڽ���Ϣ���д�С��
                    (OS_TICK        )0,					//3*5ms		//ʱ��Ƭ��ת����ʱ��
                    (void          *)0,										//�û�����洢��
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//����ѡ��������
                    (OS_ERR        *)&err
									);							
	//����7
		OSTaskCreate (
										(OS_TCB        *)&TASK7_TaskTCB,			//������ƿ�
                    (CPU_CHAR      *)"task7 task",				//��������
                    (OS_TASK_PTR    )task7_task,					//������
                    (void          *)0,										//���������ݲ���
                    (OS_PRIO        )TASK7_TASK_PRIO,			//�������ȼ�
                    (CPU_STK       *)&TASK7_TASK_STK[0],	//�����ջ����ַ ��0��ʼ
                    (CPU_STK_SIZE   )TASK7_STK_SIZE/10,		//�����ջջ������ջ��С/10
                    (CPU_STK_SIZE   )TASK7_STK_SIZE,			//�����ջ��С
                    (OS_MSG_QTY     )0,										//�����ڽ���Ϣ���д�С��
                    (OS_TICK        )0,					//3*5ms		//ʱ��Ƭ��ת����ʱ��
                    (void          *)0,										//�û�����洢��
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//����ѡ��������
                    (OS_ERR        *)&err
									);							

										
	OS_CRITICAL_EXIT();							//�˳��ٽ���
	OSTaskDel((OS_TCB*)0,&err);			//ɾ��start_task������������Ϊ0
}


void task1_task(void *p_arg)
{
	u32 task1_num=0;								//����1���д���
	OS_ERR err;											//����������
	CPU_SR_ALLOC();									//�ٽ������
	p_arg = p_arg;									//ȥ������δʹ�þ���
	POINT_COLOR = BLACK;

	OS_CRITICAL_ENTER();										//�����ٽ���
	LCD_DrawRectangle(65,110,175,314); 			//��һ������	
	LCD_DrawRectangle(200,344,310,400);  			//��һ������
	LCD_DrawLine(65,130,175,130);						//����
	POINT_COLOR = RED;
	LCD_ShowString(66,111,110,16,16,"Task1 Run:000");
	LCD_ShowString(200,324,110,16,16,"share_resource");
	LCD_Fill(66,131,174,314,BRED); //�������

	OS_CRITICAL_EXIT();							//�˳��ٽ���
	while(1)
	{
		task1_num++;
		LCD_ShowxNum(146,111,task1_num,3,16,0x80);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ2s
		
		//LCD_Fill(201,345,309,399,RED); //�������
		
		if(task1_num == 5)					//����1ִ��5�κ��������2
		{
			OSTaskSuspend((OS_TCB*)&TASK2_TaskTCB,&err);
			LCD_ShowString(201,131,110,32,16,"Task2 state:");
			LCD_ShowString(201,151,110,32,16,"Pending!");

		}
		if(task1_num == 10)				//����1����10�λָ�����2
		{
			OSTaskResume((OS_TCB*)&TASK2_TaskTCB,&err);
			LCD_ShowString(201,151,110,32,16,"Running!");
		}
		/*if(task1_num == 15)
		{
			OSTaskDel((OS_TCB*)&TASK2_TaskTCB,&err);	//����2ִ��5��ɾ������1
			LCD_ShowString(201,151,110,32,16,"Delete!");
		}*/
	}
}

void task2_task(void *p_arg)
{
	u32 task2_num = 0;							//����2���д���
	OS_ERR err;											//����������
	CPU_SR_ALLOC();									//�ٽ������
	p_arg = p_arg;									//ȥ������δʹ�þ���
	POINT_COLOR = BLACK;

	OS_CRITICAL_ENTER();											//�����ٽ���
	LCD_DrawRectangle(200,110,310,314); 			//��һ������	
	LCD_DrawLine(200,130,310,130);						//����
	POINT_COLOR = RED;
	LCD_ShowString(201,111,110,16,16,"Task2 Run:000");
	LCD_Fill(201,131,309,313,YELLOW); 				//�������
	OS_CRITICAL_EXIT();												//�˳��ٽ���
	while(1)
	{
		task2_num++;
		LCD_ShowxNum(281,111,task2_num,3,16,0x80);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
		
		
		/*for(i=0;i<=5;i++)
		{
			printf("Task2 Running\r\n");
		}*/
	}
}

void task3_task(void *p_arg)
{
	u8 key;
	u32 task3_num=0;								//����1���д���
	OS_ERR err;											//����������
	CPU_SR_ALLOC();									//�ٽ������
	p_arg = p_arg;									//ȥ������δʹ�þ���
	POINT_COLOR = BLACK;
	//����3ͼ��
	OS_CRITICAL_ENTER();										//�����ٽ���
	LCD_DrawRectangle(335,110,445,314); 			//��һ������	
	LCD_DrawLine(335,130,445,130);						//����
	POINT_COLOR = RED;
	LCD_ShowString(336,111,110,16,16,"Task3 Run:000");
	LCD_Fill(336,131,444,313,GBLUE); //�������
	//��ʱ��1����ʱ��2ͼ��
	POINT_COLOR = BLACK;
	LCD_DrawRectangle(65,344,175,400);  			//��һ������
	POINT_COLOR = RED ;
	LCD_ShowString(66,345,110,16,16,"Timer1:");
	LCD_ShowString(66,360,110,16,16,"Timer2:");
	LCD_ShowString(66,325,110,16,16,"MY_SEM:");  		
	
	OS_CRITICAL_EXIT();							//�˳��ٽ���
	while(1)
	{
		task3_num++;
		LCD_ShowxNum(416,111,task3_num,3,16,0x80);
		key = KEY_Scan(0);
		switch(key)
		{
			case KEY0_PRES:     						//��KEY0_PRES���µĻ��򿪶�ʱ��1
				OSTmrStart(&tmr1,&err);				//������ʱ��1
				printf("������ʱ��1\r\n");
				break;
			case KEY1_PRES:									//��KEY1_PRES���µĻ��򿪶�ʱ��2
				OSTmrStart(&tmr2,&err);				//������ʱ��2
				printf("������ʱ��2\r\n");
				break;
			case KEY2_PRES:									//��KEY2_PRES���»��͹رն�ʱ��
				OSTmrStop(&tmr1,OS_OPT_TMR_NONE,0,&err);	//�رն�ʱ��1
				OSTmrStop(&tmr2,OS_OPT_TMR_NONE,0,&err);	//�رն�ʱ��2
				printf("�رն�ʱ��1��2\r\n");
				break;	
			case WKUP_PRES:									//��WKUP_PRES���»��͹رն�ʱ��
				OSSemPost(&MY_SEM,OS_OPT_POST_1,&err);			//�����ź���
				LCD_ShowxNum(120,325,MY_SEM.Ctr,3,16,0x80);		//��ʾ�ź���ֵ
				break;	

		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
		/*for(i=0;i<=5;i++)
		{
			printf("Task3 Running\r\n");
		}*/
	}
}
void task4_task(void *p_arg)
{
	u32 task4_num = 0;							//����2���д���
	OS_ERR err;											//����������
	CPU_SR_ALLOC();									//�ٽ������
	p_arg = p_arg;									//ȥ������δʹ�þ���
	

	OS_CRITICAL_ENTER();											//�����ٽ���
	POINT_COLOR = BLACK;
	OS_CRITICAL_EXIT();												//�˳��ٽ���
	while(1)
	{
		
		task4_num++;
		LCD_ShowxNum(202,345,task4_num,3,16,0x80);
		OSSemPend(&MY_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); 		//�����ź���
		LCD_ShowxNum(120,325,MY_SEM.Ctr,3,16,0x80);		//��ʾ�ź���ֵ
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); 	//��ʱ1s
		LCD_Fill(200,344,310,400,GREEN); //�������
	//	OSSemPost (&MY_SEM,OS_OPT_POST_1,&err);								//�����ź���
	}
}
void task5_task(void *p_arg)
{
	u32 task5_num = 0;							//����2���д���
	OS_ERR err;											//����������
	CPU_SR_ALLOC();									//�ٽ������
	p_arg = p_arg;									//ȥ������δʹ�þ���

	OS_CRITICAL_ENTER();											//�����ٽ���
	POINT_COLOR = BLACK;
	OS_CRITICAL_EXIT();	
	while(1)
	{
		task5_num++;
		LCD_ShowxNum(202,360,task5_num,3,16,0x80);
		OSSemPend(&MY_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); 		//�����ź���
		LCD_ShowxNum(120,325,MY_SEM.Ctr,3,16,0x80);		//��ʾ�ź���ֵ
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); 	//��ʱ1s
		LCD_Fill(200,344,310,400,RED); 												//�������
		//OSSemPost (&MY_SEM,OS_OPT_POST_1,&err);								//�����ź���
	}
}
void task6_task(void *p_arg)
{
	u32 task6_num = 0;							//����2���д���
	u8 key = 0;
	OS_ERR err;											//����������
	CPU_SR_ALLOC();									//�ٽ������
	p_arg = p_arg;									//ȥ������δʹ�þ���
	OS_CRITICAL_ENTER();											//�����ٽ���
	POINT_COLOR = BLACK;
	LCD_ShowString(335,324,110,16,16,"KEY_Msg");
	LCD_DrawRectangle(335,344,445,400);  			//��һ������

	OS_CRITICAL_EXIT();	
	while(1)
	{
		task6_num++;
		LCD_ShowxNum(416,345,task6_num,3,16,0x80);
		key = TPAD_Scan();
		if(key)
		{
			//������Ϣ
			OSQPost((OS_Q*				)&KEY_Msg,						//��Ϣ��������
							(void*				)&key,								//���͵���Ϣ����
							(OS_MSG_SIZE	)1,										//��Ϣ��С
							(OS_OPT				)OS_OPT_POST_FIFO,		//�Ƚ��ȳ�
							(OS_ERR*			)&err);								//����
		}
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); 	//��ʱ1s
	}	
}

void task7_task(void *p_arg)
{
	u32 task7_num = 0;							//����2���д���
	u8 *key = 0;
	OS_MSG_SIZE	size;
	OS_ERR err;											//����������
	CPU_SR_ALLOC();									//�ٽ������
	p_arg = p_arg;									//ȥ������δʹ�þ���

	OS_CRITICAL_ENTER();											//�����ٽ���
	POINT_COLOR = BLACK;
	OS_CRITICAL_EXIT();	
	while(1)
	{
		task7_num++;
		LCD_ShowxNum(416,360,task7_num,3,16,0x80);
		//����������Ϣ
		key = OSQPend(
										(OS_Q*				)&KEY_Msg,   
										(OS_TICK			)0,
										(OS_OPT				)OS_OPT_PEND_BLOCKING,
										(OS_MSG_SIZE *)&size,		
										(CPU_TS*			)0,
										(OS_ERR*			)&err
								);
		if(*key)
		{
			LCD_Fill(336,345,444,399,lcd_discolor[task7_num%14]); 												//�������
		}

		
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); 	//��ʱ1s

	}
}

/******************************************************************
�ص�����
******************************************************************/
//��ʱ��1
void tmr1_callback(void *p_tmr, void *p_arg)
{
	static  u8 tmr1_num = 0;
	tmr1_num++;
	LCD_ShowxNum(120,345,tmr1_num,3,16,0x80);
}
//��ʱ��2
void tmr2_callback(void *p_tmr, void *p_arg)
{
	static  u8 tmr2_num = 0;
	tmr2_num++;
	LCD_ShowxNum(120,360,tmr2_num,3,16,0x80);
}


