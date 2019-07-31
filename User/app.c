#include "os.h"
#include "ARMCM3.h"

#define  TASK1_STK_SIZE       128        
#define  TASK2_STK_SIZE       128 //堆栈	大小
#define  TASK3_STK_SIZE       128 

static   CPU_STK   Task1Stk[TASK1_STK_SIZE];     //定义堆栈
static   CPU_STK   Task2Stk[TASK2_STK_SIZE]; 
static   CPU_STK   Task3Stk[TASK3_STK_SIZE];

static   OS_TCB    Task1TCB; 
static   OS_TCB    Task2TCB; 
static   OS_TCB    Task3TCB; 


void Task1(void *p_arg);
void Task2(void *p_arg);
void  Task3( void *p_arg );


unsigned int flag1;                  
unsigned int flag2; 
unsigned int flag3;
unsigned int flag4;
unsigned int flag5;

///* 将任务加入到就绪列表 */ 
//OSRdyList[0].HeadPtr = &Task1TCB;            
//OSRdyList[1].HeadPtr = &Task2TCB;   

uint32_t TimeStart;        /* 定义三个全局变量 */ 
uint32_t TimeEnd; 
uint32_t TimeUse; 

/*
************************************************************************************************************************
*                                                    main函数
************************************************************************************************************************
*/
/*
* 注意事项：1、该工程使用软件仿真，debug需选择 Ude Simulator
*           2、在Target选项卡里面把晶振Xtal(Mhz)的值改为25，默认是12，
*              改成25是为了跟system_ARMCM3.c中定义的__SYSTEM_CLOCK相同，确保仿真的时候时钟一致
*/
int main(void)
{	
	OS_ERR err;
	
	 CPU_Init();
	
	/* 关闭中断 */
	CPU_IntDis();
	
	/* 配置SysTick 10ms 中断一次 */
	OS_CPU_SysTickInit (10);
	
	/* 初始化相关的全局变量 */
	OSInit(&err);
	
	/* 创建任务 */
	OSTaskCreate ((OS_TCB*)      &Task1TCB, 
	              (OS_TASK_PTR ) Task1, 
	              (void *)       0,
					  (OS_PRIO      )1,
	              (CPU_STK*)     &Task1Stk[0],
	              (CPU_STK_SIZE) TASK1_STK_SIZE,
					  (OS_TICK      ) 0,
	              (OS_ERR *)     &err);

	OSTaskCreate ((OS_TCB*)      &Task2TCB, 
	              (OS_TASK_PTR ) Task2, 
	              (void *)       0,
						(OS_PRIO)      2,
	              (CPU_STK *)    &Task2Stk[0],
	              (CPU_STK_SIZE) TASK2_STK_SIZE,
					  (OS_TICK      ) 0,
	              (OS_ERR *)     &err);
								
		OSTaskCreate((OS_TCB      *)    &Task3TCB, 
	                (OS_TASK_PTR  ) Task3, 
	                (void *       ) 0,
					    (OS_PRIO      ) 3,
	                (CPU_STK     *) &Task3Stk[0],
	                (CPU_STK_SIZE ) TASK3_STK_SIZE,
						 (OS_TICK      ) 0,
	                (OS_ERR      *)  &err);
#if 0				  
	/* 将任务加入到就绪列表 */
	OSRdyList[0].HeadPtr = &Task1TCB;
	OSRdyList[1].HeadPtr = &Task2TCB;
#endif
	
	/* 启动OS，将不再返回 */				
	OSStart(&err);
}

void delay(unsigned int times)
{
	while(times--);
}
	

void Task1(void *p_arg)
{
	while(1)
	{
		flag1 = 1;
		flag3 = 0;
		OSTimeDly(2);
		flag1 = 0;
		flag3 = 1;
		OSTimeDly(2);
//		OSSched();
	}
}

void Task2(void *p_arg)
{
	while(1)
	{
		flag2 = 1;
		flag4 = 0;
//		TimeStart = OS_TS_GET(); 
		OSTimeDly(2);
//		TimeEnd = OS_TS_GET();        
//		TimeUse = TimeEnd - TimeStart; 
		flag2 = 0;
		flag4 = 1;
		OSTimeDly(2);
//		OSSched();
	}
}

void  Task3(void *p_arg)
{
	while(1)
	{
		flag5 = 1;
		OSTimeDly(2);
		flag5 = 0;
		OSTimeDly(2);
	}
}
