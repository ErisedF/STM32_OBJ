#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "cap_key.h"
#include "includes.h"
#include "os_app_hooks.h"

//ALIENTEK 探索者STM32F407开发板 UCOSIII实验
//例4-1 UCOSIII UCOSIII移植

//UCOSIII中以下优先级用户程序不能使用
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()


/******************************************************************
开始任务定义
******************************************************************/
#define 	START_TASK_PRIO				3						//任务优先级
#define		START_STK_SIZE				128					//任务堆栈大小128*4byte
OS_TCB		StartTaskTCB;											//任务控制块
CPU_STK		START_TASK_STK[START_STK_SIZE];		//任务堆栈
void start_task(void *p_arg);								//任务函数
/******************************************************************
任务1定义
******************************************************************/
#define 	TASK1_TASK_PRIO				4						//任务优先级
#define		TASK1_STK_SIZE				128					//任务堆栈大小128*4byte
OS_TCB		TASK1_TaskTCB;											//任务控制块
CPU_STK		TASK1_TASK_STK[TASK1_STK_SIZE];		//任务堆栈
void task1_task(void *p_arg);								//任务函数
/******************************************************************
任务2定义
******************************************************************/
#define 	TASK2_TASK_PRIO				5						//任务优先级
#define		TASK2_STK_SIZE				128					//任务堆栈大小128*4byte
OS_TCB		TASK2_TaskTCB;											//任务控制块
CPU_STK		TASK2_TASK_STK[TASK2_STK_SIZE];		//任务堆栈
void task2_task(void *p_arg);								//任务函数
/******************************************************************
任务3定义
******************************************************************/
#define 	TASK3_TASK_PRIO				6						//任务优先级
#define		TASK3_STK_SIZE				128					//任务堆栈大小128*4byte
OS_TCB		TASK3_TaskTCB;											//任务控制块
CPU_STK		TASK3_TASK_STK[TASK3_STK_SIZE];		//任务堆栈
void task3_task(void *p_arg);								//任务函数
/******************************************************************
任务4定义
******************************************************************/
#define 	TASK4_TASK_PRIO				7						//任务优先级
#define		TASK4_STK_SIZE				128					//任务堆栈大小128*4byte
OS_TCB		TASK4_TaskTCB;											//任务控制块
CPU_STK		TASK4_TASK_STK[TASK4_STK_SIZE];		//任务堆栈
void task4_task(void *p_arg);								//任务函数
/******************************************************************
任务5定义
******************************************************************/
#define 	TASK5_TASK_PRIO				8						//任务优先级
#define		TASK5_STK_SIZE				128					//任务堆栈大小128*4byte
OS_TCB		TASK5_TaskTCB;											//任务控制块
CPU_STK		TASK5_TASK_STK[TASK5_STK_SIZE];		//任务堆栈
void task5_task(void *p_arg);								//任务函数
/******************************************************************
任务6定义
******************************************************************/
#define 	TASK6_TASK_PRIO				9						//任务优先级
#define		TASK6_STK_SIZE				128					//任务堆栈大小128*4byte
OS_TCB		TASK6_TaskTCB;											//任务控制块
CPU_STK		TASK6_TASK_STK[TASK6_STK_SIZE];		//任务堆栈
void task6_task(void *p_arg);								//任务函数
/******************************************************************
任务7定义
******************************************************************/
#define 	TASK7_TASK_PRIO				10						//任务优先级
#define		TASK7_STK_SIZE				128					//任务堆栈大小128*4byte
OS_TCB		TASK7_TaskTCB;											//任务控制块
CPU_STK		TASK7_TASK_STK[TASK7_STK_SIZE];		//任务堆栈
void task7_task(void *p_arg);								//任务函数


/******************************************************************
定义定时器
******************************************************************/
OS_TMR 	tmr1;		//定时器1结构体
OS_TMR	tmr2;		//定时器2结构体
void tmr1_callback(void *p_tmr, void *p_arg); 	//定时器1回调函数
void tmr2_callback(void *p_tmr, void *p_arg);		//定时器2回调函数


/******************************************************************
定义信号量
******************************************************************/
OS_SEM	MY_SEM;		//定义一个信号量，用于访问共享资源


/******************************************************************
定义消息队列
******************************************************************/
#define KEYMSG_Q_NUM	1	//按键消息队列的数量
OS_Q KEY_Msg;						//定义一个消息队列

/******************************************************************
定义刷屏颜色
******************************************************************/
int lcd_discolor[14]={	WHITE, BLACK, BLUE,  BRED,      
						GRED,  GBLUE, RED,   MAGENTA,       	 
						GREEN, CYAN,  YELLOW,BROWN, 			
						BRRED, GRAY };


int main(void)
{
	OS_ERR err;																						//定义错误变量
	CPU_SR_ALLOC();																				//临界段配置
	delay_init(168);																			//延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//中断分组
	uart_init(115200);																		//USART初始化
	LED_Init();																						//LED初始化
	LCD_Init();																						//LCD初始化
	KEY_Init();																						//按键初始化
	TPAD_Init();																					//电容按键初始化

/******************************************************************
LCD显示设置
******************************************************************/
	POINT_COLOR = RED;
	LCD_ShowString(100,20,266,24,24,"UCOSIII_TASK_CREAT_DEL");		
/******************************************************************
在临界区中创建任务
******************************************************************/
	OSInit(&err);				//UCOSIII初始化
	OS_CRITICAL_ENTER();	//进入临界区
	OSTaskCreate (
										(OS_TCB        *)&StartTaskTCB,				//任务控制块
                    (CPU_CHAR      *)"start task",				//任务名字
                    (OS_TASK_PTR    )start_task,					//任务函数
                    (void          *)0,										//任务函数传递参数
                    (OS_PRIO        )START_TASK_PRIO,			//任务优先级
                    (CPU_STK       *)&START_TASK_STK[0],	//任务堆栈基地址 从0开始
                    (CPU_STK_SIZE   )START_STK_SIZE/10,		//任务堆栈栈深，任务堆栈大小/10
                    (CPU_STK_SIZE   )START_STK_SIZE,			//任务堆栈大小
                    (OS_MSG_QTY     )0,										//任务内建消息队列大小，
                    (OS_TICK        )0,										//时间片轮转调度时间
                    (void          *)0,										//用户补充存储区
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//任务选项，检查和清除
                    (OS_ERR        *)&err
									);
	OS_CRITICAL_EXIT();		//退出临界区
	OSStart(&err);
}

/******************************************************************
开始任务函数
在开始任务函数中创建新的任务函数
******************************************************************/
void start_task(void *p_arg)
{
	OS_ERR err;											//定义错误变量
	CPU_SR_ALLOC();									//临界段配置
	p_arg = p_arg;									//去除变量未使用警告
	CPU_Init();											//CPU初始化
	
	
/******************************************************************
功能条件编译
******************************************************************/
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN			//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  		//当使用时间片轮转的时候
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
#endif		
	

	OS_CRITICAL_ENTER();	//进入临界区
/******************************************************************
在临界区中创建信号量
******************************************************************/
	OSSemCreate (
										(OS_SEM      *)&MY_SEM,				//指向信号量量控制块
										(CPU_CHAR    *)"MY_SEM",			//信号量名字
										(OS_SEM_CTR   )0,							//信号量初始值
										(OS_ERR      *)&err						//错误代码
									 );
/******************************************************************
在临界区中创建消息队列
******************************************************************/
	OSQCreate (
										(OS_Q					*)&KEY_Msg,				//消息队列
										(CPU_CHAR	 	  *)"KEY Msg",			//消息队列名称
										(OS_MSG_QTY		 )KEYMSG_Q_NUM,		//消息队列长度，这里设置为1
										(OS_ERR		  	*)&err 						//错误码
						);		


/******************************************************************
在临界区中创建定时器1、定时器2
******************************************************************/
	//定时器1
	OSTmrCreate (
									 (OS_TMR               *)&tmr1,									//指定定时器
                   (CPU_CHAR             *)"tmr1",								//定时器名字
                   (OS_TICK               )20,										//初始延时时间 20*10 = 200ms
                   (OS_TICK               )100,										//周期模式延时时间 100*10 = 1000ms
                   (OS_OPT                )OS_OPT_TMR_PERIODIC,		//定时器模式：周期模式
                   (OS_TMR_CALLBACK_PTR   )tmr1_callback,					//回调函数
                   (void                 *)0,											//参数为0
                   (OS_ERR               *)&err										//返回的错误码
							);
	//定时器2
	OSTmrCreate (
									 (OS_TMR               *)&tmr2,									//指定定时器
                   (CPU_CHAR             *)"tmr2",								//定时器名字
                   (OS_TICK               )200,										//初始延时时间 200*10 = 2000ms
                   (OS_TICK               )0,											//周期模式延时时间 100*100 = 10000ms
                   (OS_OPT                )OS_OPT_TMR_ONE_SHOT,		//定时器模式：单次模式
                   (OS_TMR_CALLBACK_PTR   )tmr2_callback,					//回调函数
                   (void                 *)0,											//参数为0
                   (OS_ERR               *)&err										//返回的错误码
							);

/******************************************************************
在临界区中创建任务1、任务2、任务3
******************************************************************/
	//任务1
	OSTaskCreate (
										(OS_TCB        *)&TASK1_TaskTCB,			//任务控制块
                    (CPU_CHAR      *)"task1 task",				//任务名字
                    (OS_TASK_PTR    )task1_task,					//任务函数
                    (void          *)0,										//任务函数传递参数
                    (OS_PRIO        )TASK1_TASK_PRIO,			//任务优先级
                    (CPU_STK       *)&TASK1_TASK_STK[0],	//任务堆栈基地址 从0开始
                    (CPU_STK_SIZE   )TASK1_STK_SIZE/10,		//任务堆栈栈深，任务堆栈大小/10
                    (CPU_STK_SIZE   )TASK1_STK_SIZE,			//任务堆栈大小
                    (OS_MSG_QTY     )0,										//任务内建消息队列大小，
                    (OS_TICK        )0,						//0*5ms	//时间片轮转调度时间
                    (void          *)0,										//用户补充存储区
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//任务选项，检查和清除
                    (OS_ERR        *)&err
									);
	//任务2
		OSTaskCreate (
										(OS_TCB        *)&TASK2_TaskTCB,			//任务控制块
                    (CPU_CHAR      *)"task2 task",				//任务名字
                    (OS_TASK_PTR    )task2_task,					//任务函数
                    (void          *)0,										//任务函数传递参数
                    (OS_PRIO        )TASK2_TASK_PRIO,			//任务优先级
                    (CPU_STK       *)&TASK2_TASK_STK[0],	//任务堆栈基地址 从0开始
                    (CPU_STK_SIZE   )TASK2_STK_SIZE/10,		//任务堆栈栈深，任务堆栈大小/10
                    (CPU_STK_SIZE   )TASK2_STK_SIZE,			//任务堆栈大小
                    (OS_MSG_QTY     )0,										//任务内建消息队列大小，
                    (OS_TICK        )0,					//3*5ms		//时间片轮转调度时间
                    (void          *)0,										//用户补充存储区
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//任务选项，检查和清除
                    (OS_ERR        *)&err
									);	
	//任务3
		OSTaskCreate (
										(OS_TCB        *)&TASK3_TaskTCB,			//任务控制块
                    (CPU_CHAR      *)"task3 task",				//任务名字
                    (OS_TASK_PTR    )task3_task,					//任务函数
                    (void          *)0,										//任务函数传递参数
                    (OS_PRIO        )TASK3_TASK_PRIO,			//任务优先级
                    (CPU_STK       *)&TASK3_TASK_STK[0],	//任务堆栈基地址 从0开始
                    (CPU_STK_SIZE   )TASK3_STK_SIZE/10,		//任务堆栈栈深，任务堆栈大小/10
                    (CPU_STK_SIZE   )TASK3_STK_SIZE,			//任务堆栈大小
                    (OS_MSG_QTY     )0,										//任务内建消息队列大小，
                    (OS_TICK        )0,					//3*5ms		//时间片轮转调度时间
                    (void          *)0,										//用户补充存储区
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//任务选项，检查和清除
                    (OS_ERR        *)&err
									);							
	//任务4
		OSTaskCreate (
										(OS_TCB        *)&TASK4_TaskTCB,			//任务控制块
                    (CPU_CHAR      *)"task4 task",				//任务名字
                    (OS_TASK_PTR    )task4_task,					//任务函数
                    (void          *)0,										//任务函数传递参数
                    (OS_PRIO        )TASK4_TASK_PRIO,			//任务优先级
                    (CPU_STK       *)&TASK4_TASK_STK[0],	//任务堆栈基地址 从0开始
                    (CPU_STK_SIZE   )TASK4_STK_SIZE/10,		//任务堆栈栈深，任务堆栈大小/10
                    (CPU_STK_SIZE   )TASK4_STK_SIZE,			//任务堆栈大小
                    (OS_MSG_QTY     )0,										//任务内建消息队列大小，
                    (OS_TICK        )0,					//3*5ms		//时间片轮转调度时间
                    (void          *)0,										//用户补充存储区
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//任务选项，检查和清除
                    (OS_ERR        *)&err
									);
	//任务5
		OSTaskCreate (
										(OS_TCB        *)&TASK5_TaskTCB,			//任务控制块
                    (CPU_CHAR      *)"task5 task",				//任务名字
                    (OS_TASK_PTR    )task5_task,					//任务函数
                    (void          *)0,										//任务函数传递参数
                    (OS_PRIO        )TASK5_TASK_PRIO,			//任务优先级
                    (CPU_STK       *)&TASK5_TASK_STK[0],	//任务堆栈基地址 从0开始
                    (CPU_STK_SIZE   )TASK5_STK_SIZE/10,		//任务堆栈栈深，任务堆栈大小/10
                    (CPU_STK_SIZE   )TASK5_STK_SIZE,			//任务堆栈大小
                    (OS_MSG_QTY     )0,										//任务内建消息队列大小，
                    (OS_TICK        )0,					//3*5ms		//时间片轮转调度时间
                    (void          *)0,										//用户补充存储区
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//任务选项，检查和清除
                    (OS_ERR        *)&err
									);							
	//任务6
		OSTaskCreate (
										(OS_TCB        *)&TASK6_TaskTCB,			//任务控制块
                    (CPU_CHAR      *)"task6 task",				//任务名字
                    (OS_TASK_PTR    )task6_task,					//任务函数
                    (void          *)0,										//任务函数传递参数
                    (OS_PRIO        )TASK6_TASK_PRIO,			//任务优先级
                    (CPU_STK       *)&TASK6_TASK_STK[0],	//任务堆栈基地址 从0开始
                    (CPU_STK_SIZE   )TASK6_STK_SIZE/10,		//任务堆栈栈深，任务堆栈大小/10
                    (CPU_STK_SIZE   )TASK6_STK_SIZE,			//任务堆栈大小
                    (OS_MSG_QTY     )0,										//任务内建消息队列大小，
                    (OS_TICK        )0,					//3*5ms		//时间片轮转调度时间
                    (void          *)0,										//用户补充存储区
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//任务选项，检查和清除
                    (OS_ERR        *)&err
									);							
	//任务7
		OSTaskCreate (
										(OS_TCB        *)&TASK7_TaskTCB,			//任务控制块
                    (CPU_CHAR      *)"task7 task",				//任务名字
                    (OS_TASK_PTR    )task7_task,					//任务函数
                    (void          *)0,										//任务函数传递参数
                    (OS_PRIO        )TASK7_TASK_PRIO,			//任务优先级
                    (CPU_STK       *)&TASK7_TASK_STK[0],	//任务堆栈基地址 从0开始
                    (CPU_STK_SIZE   )TASK7_STK_SIZE/10,		//任务堆栈栈深，任务堆栈大小/10
                    (CPU_STK_SIZE   )TASK7_STK_SIZE,			//任务堆栈大小
                    (OS_MSG_QTY     )0,										//任务内建消息队列大小，
                    (OS_TICK        )0,					//3*5ms		//时间片轮转调度时间
                    (void          *)0,										//用户补充存储区
                    (OS_OPT         )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,		//任务选项，检查和清除
                    (OS_ERR        *)&err
									);							

										
	OS_CRITICAL_EXIT();							//退出临界区
	OSTaskDel((OS_TCB*)0,&err);			//删除start_task任务自身，参数为0
}


void task1_task(void *p_arg)
{
	u32 task1_num=0;								//任务1运行次数
	OS_ERR err;											//定义错误变量
	CPU_SR_ALLOC();									//临界段配置
	p_arg = p_arg;									//去除变量未使用警告
	POINT_COLOR = BLACK;

	OS_CRITICAL_ENTER();										//进入临界区
	LCD_DrawRectangle(65,110,175,314); 			//画一个矩形	
	LCD_DrawRectangle(200,344,310,400);  			//画一个矩形
	LCD_DrawLine(65,130,175,130);						//画线
	POINT_COLOR = RED;
	LCD_ShowString(66,111,110,16,16,"Task1 Run:000");
	LCD_ShowString(200,324,110,16,16,"share_resource");
	LCD_Fill(66,131,174,314,BRED); //填充区域

	OS_CRITICAL_EXIT();							//退出临界区
	while(1)
	{
		task1_num++;
		LCD_ShowxNum(146,111,task1_num,3,16,0x80);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //延时2s
		
		//LCD_Fill(201,345,309,399,RED); //填充区域
		
		if(task1_num == 5)					//任务1执行5次后挂起任务2
		{
			OSTaskSuspend((OS_TCB*)&TASK2_TaskTCB,&err);
			LCD_ShowString(201,131,110,32,16,"Task2 state:");
			LCD_ShowString(201,151,110,32,16,"Pending!");

		}
		if(task1_num == 10)				//任务1运行10次恢复任务2
		{
			OSTaskResume((OS_TCB*)&TASK2_TaskTCB,&err);
			LCD_ShowString(201,151,110,32,16,"Running!");
		}
		/*if(task1_num == 15)
		{
			OSTaskDel((OS_TCB*)&TASK2_TaskTCB,&err);	//任务2执行5次删除任务1
			LCD_ShowString(201,151,110,32,16,"Delete!");
		}*/
	}
}

void task2_task(void *p_arg)
{
	u32 task2_num = 0;							//任务2运行次数
	OS_ERR err;											//定义错误变量
	CPU_SR_ALLOC();									//临界段配置
	p_arg = p_arg;									//去除变量未使用警告
	POINT_COLOR = BLACK;

	OS_CRITICAL_ENTER();											//进入临界区
	LCD_DrawRectangle(200,110,310,314); 			//画一个矩形	
	LCD_DrawLine(200,130,310,130);						//画线
	POINT_COLOR = RED;
	LCD_ShowString(201,111,110,16,16,"Task2 Run:000");
	LCD_Fill(201,131,309,313,YELLOW); 				//填充区域
	OS_CRITICAL_EXIT();												//退出临界区
	while(1)
	{
		task2_num++;
		LCD_ShowxNum(281,111,task2_num,3,16,0x80);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
		
		
		/*for(i=0;i<=5;i++)
		{
			printf("Task2 Running\r\n");
		}*/
	}
}

void task3_task(void *p_arg)
{
	u8 key;
	u32 task3_num=0;								//任务1运行次数
	OS_ERR err;											//定义错误变量
	CPU_SR_ALLOC();									//临界段配置
	p_arg = p_arg;									//去除变量未使用警告
	POINT_COLOR = BLACK;
	//任务3图形
	OS_CRITICAL_ENTER();										//进入临界区
	LCD_DrawRectangle(335,110,445,314); 			//画一个矩形	
	LCD_DrawLine(335,130,445,130);						//画线
	POINT_COLOR = RED;
	LCD_ShowString(336,111,110,16,16,"Task3 Run:000");
	LCD_Fill(336,131,444,313,GBLUE); //填充区域
	//定时器1、定时器2图形
	POINT_COLOR = BLACK;
	LCD_DrawRectangle(65,344,175,400);  			//画一个矩形
	POINT_COLOR = RED ;
	LCD_ShowString(66,345,110,16,16,"Timer1:");
	LCD_ShowString(66,360,110,16,16,"Timer2:");
	LCD_ShowString(66,325,110,16,16,"MY_SEM:");  		
	
	OS_CRITICAL_EXIT();							//退出临界区
	while(1)
	{
		task3_num++;
		LCD_ShowxNum(416,111,task3_num,3,16,0x80);
		key = KEY_Scan(0);
		switch(key)
		{
			case KEY0_PRES:     						//当KEY0_PRES按下的话打开定时器1
				OSTmrStart(&tmr1,&err);				//开启定时器1
				printf("开启定时器1\r\n");
				break;
			case KEY1_PRES:									//当KEY1_PRES按下的话打开定时器2
				OSTmrStart(&tmr2,&err);				//开启定时器2
				printf("开启定时器2\r\n");
				break;
			case KEY2_PRES:									//当KEY2_PRES按下话就关闭定时器
				OSTmrStop(&tmr1,OS_OPT_TMR_NONE,0,&err);	//关闭定时器1
				OSTmrStop(&tmr2,OS_OPT_TMR_NONE,0,&err);	//关闭定时器2
				printf("关闭定时器1和2\r\n");
				break;	
			case WKUP_PRES:									//当WKUP_PRES按下话就关闭定时器
				OSSemPost(&MY_SEM,OS_OPT_POST_1,&err);			//发送信号量
				LCD_ShowxNum(120,325,MY_SEM.Ctr,3,16,0x80);		//显示信号量值
				break;	

		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
		/*for(i=0;i<=5;i++)
		{
			printf("Task3 Running\r\n");
		}*/
	}
}
void task4_task(void *p_arg)
{
	u32 task4_num = 0;							//任务2运行次数
	OS_ERR err;											//定义错误变量
	CPU_SR_ALLOC();									//临界段配置
	p_arg = p_arg;									//去除变量未使用警告
	

	OS_CRITICAL_ENTER();											//进入临界区
	POINT_COLOR = BLACK;
	OS_CRITICAL_EXIT();												//退出临界区
	while(1)
	{
		
		task4_num++;
		LCD_ShowxNum(202,345,task4_num,3,16,0x80);
		OSSemPend(&MY_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); 		//请求信号量
		LCD_ShowxNum(120,325,MY_SEM.Ctr,3,16,0x80);		//显示信号量值
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); 	//延时1s
		LCD_Fill(200,344,310,400,GREEN); //填充区域
	//	OSSemPost (&MY_SEM,OS_OPT_POST_1,&err);								//发送信号量
	}
}
void task5_task(void *p_arg)
{
	u32 task5_num = 0;							//任务2运行次数
	OS_ERR err;											//定义错误变量
	CPU_SR_ALLOC();									//临界段配置
	p_arg = p_arg;									//去除变量未使用警告

	OS_CRITICAL_ENTER();											//进入临界区
	POINT_COLOR = BLACK;
	OS_CRITICAL_EXIT();	
	while(1)
	{
		task5_num++;
		LCD_ShowxNum(202,360,task5_num,3,16,0x80);
		OSSemPend(&MY_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); 		//请求信号量
		LCD_ShowxNum(120,325,MY_SEM.Ctr,3,16,0x80);		//显示信号量值
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); 	//延时1s
		LCD_Fill(200,344,310,400,RED); 												//填充区域
		//OSSemPost (&MY_SEM,OS_OPT_POST_1,&err);								//发送信号量
	}
}
void task6_task(void *p_arg)
{
	u32 task6_num = 0;							//任务2运行次数
	u8 key = 0;
	OS_ERR err;											//定义错误变量
	CPU_SR_ALLOC();									//临界段配置
	p_arg = p_arg;									//去除变量未使用警告
	OS_CRITICAL_ENTER();											//进入临界区
	POINT_COLOR = BLACK;
	LCD_ShowString(335,324,110,16,16,"KEY_Msg");
	LCD_DrawRectangle(335,344,445,400);  			//画一个矩形

	OS_CRITICAL_EXIT();	
	while(1)
	{
		task6_num++;
		LCD_ShowxNum(416,345,task6_num,3,16,0x80);
		key = TPAD_Scan();
		if(key)
		{
			//发送消息
			OSQPost((OS_Q*				)&KEY_Msg,						//消息队列名字
							(void*				)&key,								//发送的消息内容
							(OS_MSG_SIZE	)1,										//消息大小
							(OS_OPT				)OS_OPT_POST_FIFO,		//先进先出
							(OS_ERR*			)&err);								//错误
		}
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); 	//延时1s
	}	
}

void task7_task(void *p_arg)
{
	u32 task7_num = 0;							//任务2运行次数
	u8 *key = 0;
	OS_MSG_SIZE	size;
	OS_ERR err;											//定义错误变量
	CPU_SR_ALLOC();									//临界段配置
	p_arg = p_arg;									//去除变量未使用警告

	OS_CRITICAL_ENTER();											//进入临界区
	POINT_COLOR = BLACK;
	OS_CRITICAL_EXIT();	
	while(1)
	{
		task7_num++;
		LCD_ShowxNum(416,360,task7_num,3,16,0x80);
		//请求任务消息
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
			LCD_Fill(336,345,444,399,lcd_discolor[task7_num%14]); 												//填充区域
		}

		
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); 	//延时1s

	}
}

/******************************************************************
回调函数
******************************************************************/
//定时器1
void tmr1_callback(void *p_tmr, void *p_arg)
{
	static  u8 tmr1_num = 0;
	tmr1_num++;
	LCD_ShowxNum(120,345,tmr1_num,3,16,0x80);
}
//定时器2
void tmr2_callback(void *p_tmr, void *p_arg)
{
	static  u8 tmr2_num = 0;
	tmr2_num++;
	LCD_ShowxNum(120,360,tmr2_num,3,16,0x80);
}


