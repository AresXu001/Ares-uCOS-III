#include "os.h"
#include "ARMCM3.h"

#define  TASK1_STK_SIZE       128        
#define  TASK2_STK_SIZE       128    //��ջ	��С


static   CPU_STK   Task1Stk[TASK1_STK_SIZE];     //�����ջ
static   CPU_STK   Task2Stk[TASK2_STK_SIZE]; 

static   OS_TCB    Task1TCB; 
static   OS_TCB    Task2TCB; 

void Task1(void *p_arg);
void Task2(void *p_arg);

unsigned int flag1;                  
unsigned int flag2; 
unsigned int flag3;
unsigned int flag4;

///* ��������뵽�����б� */ 
//OSRdyList[0].HeadPtr = &Task1TCB;            
//OSRdyList[1].HeadPtr = &Task2TCB;   

uint32_t TimeStart;        /* ��������ȫ�ֱ��� */ 
uint32_t TimeEnd; 
uint32_t TimeUse; 

/*
************************************************************************************************************************
*                                                    main����
************************************************************************************************************************
*/
/*
* ע�����1���ù���ʹ��������棬debug��ѡ�� Ude Simulator
*           2����Targetѡ�����Ѿ���Xtal(Mhz)��ֵ��Ϊ25��Ĭ����12��
*              �ĳ�25��Ϊ�˸�system_ARMCM3.c�ж����__SYSTEM_CLOCK��ͬ��ȷ�������ʱ��ʱ��һ��
*/
int main(void)
{	
	OS_ERR err;
	
	 CPU_Init();
	
	/* �ر��ж� */
	CPU_IntDis();
	
	/* ����SysTick 10ms �ж�һ�� */
	OS_CPU_SysTickInit (10);
	
	/* ��ʼ����ص�ȫ�ֱ��� */
	OSInit(&err);
	
	/* �������� */
	OSTaskCreate ((OS_TCB*)      &Task1TCB, 
	              (OS_TASK_PTR ) Task1, 
	              (void *)       0,
	              (CPU_STK*)     &Task1Stk[0],
	              (CPU_STK_SIZE) TASK1_STK_SIZE,
	              (OS_ERR *)     &err);

	OSTaskCreate ((OS_TCB*)      &Task2TCB, 
	              (OS_TASK_PTR ) Task2, 
	              (void *)       0,
	              (CPU_STK*)     &Task2Stk[0],
	              (CPU_STK_SIZE) TASK2_STK_SIZE,
	              (OS_ERR *)     &err);
				  
	/* ��������뵽�����б� */
	OSRdyList[0].HeadPtr = &Task1TCB;
	OSRdyList[1].HeadPtr = &Task2TCB;
	
	/* ����OS�������ٷ��� */				
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
		TimeStart = OS_TS_GET(); 
		OSTimeDly(20);
		TimeEnd = OS_TS_GET();        
		TimeUse = TimeEnd - TimeStart; 
		flag2 = 0;
		flag4 = 1;
		OSTimeDly(2);
//		OSSched();
	}
}
