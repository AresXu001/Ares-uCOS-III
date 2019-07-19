#include "os.h"
#include "os_cpu.h"

#if 0
void OS_RdyListInit(void) 
{ 
	OS_PRIO i; 
	OS_RDY_LIST *p_rdy_list; 

	for (i=0u; i< OS_CFG_PRIO_MAX; i++) 
	{ 
		 p_rdy_list = &OSRdyList[i]; 
		 p_rdy_list->HeadPtr = (OS_TCB *)0; 
		 p_rdy_list->TailPtr  = (OS_TCB *)0; 
	} 
} 
#endif

void OS_IdleTaskInit(OS_ERR *p_err);
void OS_RdyListInit(void);
void OSInit (OS_ERR *p_err) 
{ 
		OSRunning       =  OS_STATE_OS_STOPPED;              

	/* ��ʼ������ȫ�ֵ�TCB,���������л� */
		OSTCBCurPtr     = (OS_TCB *)0;                     
		OSTCBHighRdyPtr = (OS_TCB *)0;  
	
  /* ��ʼ�����ȼ����� */
		OSPrioCur    = (OS_PRIO)0;
		OSPrioHighRdy = (OS_PRIO)0;
		
		/* ��ʼ�������б� */
		OS_RdyListInit();  
	
		/* ��ʼ���������� */
		OS_IdleTaskInit(p_err);   //��ʼ����������
		
		if(*p_err != OS_ERR_NONE)
		{
			return;
		}			
} 

/*��������*/
void OSStart(OS_ERR*p_err)
{
	if(OSRunning == OS_STATE_OS_STOPPED)
	{
#if 0   /* �ֶ���������1������ */      
		OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
#endif
		
		/* Ѱ��������ȼ� */
		OSPrioHighRdy = OS_PrioGetHighest();
		OSPrioCur     = OSPrioHighRdy;
		
		/* �ҵ�������ȼ������TCB */
		OSTCBHighRdyPtr = OSRdyList[OSPrioHighRdy].HeadPtr;
		OSTCBCurPtr     = OSTCBHighRdyPtr;
		
		/* ��־ϵͳ���� */
		OSRunning = OS_STATE_OS_RUNNING;  
		
		OSStartHighRdy();  /* ���������л� �����ٷ��� */
		
		*p_err = OS_ERR_FATAL_RETURN;
	}
	else
	{
		*p_err = OS_STATE_OS_RUNNING;
	}
}

/* �����л���ʵ�ʾ��Ǵ���PendSV�쳣��Ȼ����PendSV�쳣�н����������л� */
void OSSched (void)
{
#if 0  /* �򵥵�������������ִ�� */
	if( OSTCBCurPtr == OSRdyList[0].HeadPtr )
	{
		OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
	}
	else
	{
		OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
	}
#endif

#if 0
	/* �����ǰ�����ǿ������� */
	if(OSTCBCurPtr == &OSIdleTaskTCB)
	{
		if(OSRdyList[0].HeadPtr->TaskDelayTicks == 0) //�������1��ʱ�Ƿ����
		{
			OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;    //ִ������1
		}
		else if(OSRdyList[1].HeadPtr->TaskDelayTicks == 0) //�������2��ʱ�Ƿ����
		{
			OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;  //ִ������1
		}
		else
		{
			return;
		}
	}
	else 
	{
		if(OSTCBCurPtr == OSRdyList[0].HeadPtr)  //�����ǰִ������������1
		{
			if(OSRdyList[1].HeadPtr->TaskDelayTicks == 0) //�������2��ʱ�Ƿ����
			{
				OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;    //ִ������2
			}
			else if(OSTCBCurPtr->TaskDelayTicks != 0)    //��鵱ǰ������ʱ�Ƿ����
			{
				OSTCBHighRdyPtr = &OSIdleTaskTCB;         //��ʱû����ִ�п�������
			}
			else
			{
				return;  /* �������񶼴�����ʱ�У������������л� */
			}
		}
		else if(OSTCBCurPtr == OSRdyList[1].HeadPtr)
		{
			if(OSRdyList[0].HeadPtr->TaskDelayTicks == 0)
			{
				OSTCBHighRdyPtr = OSRdyList[0].HeadPtr; 
			}
			else if(OSTCBCurPtr->TaskDelayTicks != 0)
			{
				OSTCBHighRdyPtr = &OSIdleTaskTCB;
			}
			else
			{
				return;
			}
		}
	}
	OS_TASK_SW();  //�����л�
#endif
	
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();  /* �����ٽ��� */
	
	OSPrioHighRdy   = OS_PrioGetHighest();
	OSTCBHighRdyPtr = OSRdyList[OSPrioHighRdy].HeadPtr;
	
	/* ���������ȼ������ǵ�ǰ���� ֱ�ӷ��أ������������л� */
	if(OSTCBHighRdyPtr == OSTCBCurPtr)
	{ /*�˳��ٽ���*/
		OS_CRITICAL_EXIT();
		return;
	}
	
	/*�˳��ٽ���*/
	OS_CRITICAL_EXIT();
	
	OS_TASK_SW();  /* �����л� ������PendSV�쳣 */
}

//char flag5 = 0;
/* �������� */
void OS_IdleTask(void *p_arg)
{
	p_arg = p_arg;
	
	for(;;)
	{
		OSIdleTaskCtr++;
//		flag5 = 0;
//		OSTimeDly(3);
//		flag5 = 1;
//		OSTimeDly(3);
	}
}

/* ���������ʼ�� */
void OS_IdleTaskInit(OS_ERR *p_err)
{
	/* ��ʼ��������������� */
	OSIdleTaskCtr = (OS_IDLE_CTR)0;
	
	OSTaskCreate ((OS_TCB      *)  &OSIdleTaskTCB, 
	              (OS_TASK_PTR  )  OS_IdleTask, 
	              (void        *)  0,
								(OS_PRIO      )  (OS_CFG_PRIO_MAX - 1u), //�������ȼ�������ʼ��
	              (CPU_STK     *)  OSCfg_IdleTaskStkBasePtr,
	              (CPU_STK_SIZE )  OSCfg_IdleTaskStkSize,
	              (OS_ERR      *)  p_err);
}

/* ��ʼ����������б� */
void OS_RdyListInit(void)
{
	OS_PRIO i;
	OS_RDY_LIST *p_rdy_list;
	
	for(i = 0u;i < OS_CFG_PRIO_MAX;i++)
	{
		p_rdy_list = &OSRdyList[i];
		p_rdy_list->NbrEnries = (OS_OBJ_QTY)0;
		p_rdy_list->HeadPtr   = (OS_TCB *)0;
		p_rdy_list->TailPtr   = (OS_TCB *)0;
	}
}

/* ���뵽����ͷ�� */
void OS_RdyListInsertHead(OS_TCB *p_tcb)
{
	OS_RDY_LIST  *p_rdy_list;
	OS_TCB       *p_tcb2;
	
	/* ��ȡ������� */
	p_rdy_list = &OSRdyList[p_tcb->prio];
	
	/* ����Ϊ�� */
	if(p_rdy_list->NbrEnries == (OS_OBJ_QTY)0)
	{
		p_rdy_list->NbrEnries = (OS_OBJ_QTY)1;
		p_tcb->NextPtr        = (OS_TCB *)0;
		p_tcb->PrevPtr        = (OS_TCB *)0;
		p_rdy_list->HeadPtr   = p_tcb;
		p_rdy_list->TailPtr   = p_tcb;
	}
	else  /* �����Ѿ��нڵ� */
	{
		p_rdy_list->NbrEnries++;
		p_tcb->PrevPtr      = (OS_TCB *)0;     //��ͷ�ڵ�ǰ��ָ���ÿ�
		p_tcb->NextPtr      = p_rdy_list->HeadPtr; //��ͷ���ָ���ͷ���
		p_tcb2              = p_rdy_list->HeadPtr;  //��ͷ���
		p_tcb2->PrevPtr     = p_tcb;      //��ͷ�ڵ�ָ����ͷ�ڵ�
		p_rdy_list->HeadPtr = p_tcb;   //���ڵ�ͷ��ָ��ָ����ͷ���
	}
}

/* ���뵽�ı�β�� */
void OS_RdyListInsertTail(OS_TCB *p_tcb)
{
	OS_RDY_LIST  *p_rdy_list;
	OS_TCB       *p_tcb2;
	
	/* ��ȡ������� */
	p_rdy_list = &OSRdyList[p_tcb->prio];
	if(p_rdy_list->NbrEnries == (OS_OBJ_QTY)0)
	{
		p_rdy_list->NbrEnries = (OS_OBJ_QTY)1;
		p_tcb->NextPtr = (OS_TCB *)0;
		p_tcb->PrevPtr = (OS_TCB *)0;
		p_rdy_list->HeadPtr = p_tcb;
		p_rdy_list->TailPtr = p_tcb;
	}
	else
	{
		p_rdy_list->NbrEnries++;
		p_tcb->NextPtr      = (OS_TCB *)0;
		p_tcb->PrevPtr      = p_rdy_list->TailPtr;//��β���ָ���β���
		p_tcb2              = p_rdy_list->TailPtr;  //��β���
		p_tcb2->NextPtr     = p_tcb;  //��β���ָ����β���
		p_rdy_list->TailPtr = p_tcb;  //���ڵ�βָ��ָ����β���
	}
}

void OS_RdyListInsert(OS_TCB *p_tcb)
{
	OS_PrioInsert(p_tcb->prio);
	
	if(p_tcb->prio == OSPrioCur)
	{
		/* ����ǵ�ǰ���ȼ� ���뵽����β�� */
		OS_RdyListInsertTail(p_tcb);
	}
	else
	{
		/* �����������ͷ�� */
		OS_RdyListInsertHead(p_tcb);
	}
}



void OS_RdyListMoveHeadToTail(OS_RDY_LIST *p_rdy_list)
{
	OS_TCB *p_tcb_tail;
	OS_TCB *p_tcb_head;
	OS_TCB *p_tcb1;
	OS_TCB *p_tcb2;
	switch(p_rdy_list->NbrEnries)
	{
		case 0:
		case 1:
			break;
		case 2:
			p_tcb_head = p_rdy_list->HeadPtr;  //ͷ���ָ��
			p_tcb_tail = p_rdy_list->TailPtr;  //β���ָ��
			p_rdy_list->HeadPtr = p_tcb_tail;  //β����ͷ���
		  p_rdy_list->TailPtr = p_tcb_head;  //ͷ����β���
		  p_tcb_tail->NextPtr = p_tcb_head;  //��ͷ���ָ��ͷ���
			p_tcb_head->PrevPtr = p_tcb_tail;  //��ͷ���ָ����ͷ���
			p_tcb_tail->PrevPtr = (OS_TCB *)0;
			p_tcb_head->NextPtr = (OS_TCB *)0;  
		break;
		
		default:
			p_tcb_head = p_rdy_list->HeadPtr;  //ͷ���ָ��
			p_tcb_tail = p_rdy_list->TailPtr;  //β���ָ��
			p_tcb1     = p_tcb_head->NextPtr;  //ͷ���ĺ�����
			p_tcb2     = p_tcb_tail->PrevPtr;  //β����ǰ����
			
		 /* β����ͷ��� */
			p_tcb_head->NextPtr = (OS_TCB *)0;
			p_tcb_tail->PrevPtr = (OS_TCB *)0;		
			p_tcb_head->PrevPtr = p_tcb2; 
			p_tcb_tail->NextPtr = p_tcb1;			
			p_tcb1->PrevPtr = p_tcb_tail;
			p_tcb2->NextPtr = p_tcb_head;
		break;
			
	}
}


void OS_RdyListRemove(OS_TCB *p_tcb)
{
	OS_RDY_LIST * p_rdy_list;
	OS_TCB *p_tcb1;
	OS_TCB *p_tcb2;
	
	p_rdy_list = &OSRdyList[p_tcb->prio];
	
	/* ����Ҫɾ���Ľڵ��ǰ��TCB�ڵ� */
	p_tcb1 = p_tcb->NextPtr;
	p_tcb2 = p_tcb->PrevPtr;
	
	/* ɾ�����ǵ�һ���ڵ� */
	if(p_tcb1 == (OS_TCB *)0)
	{
		if(p_tcb2 == (OS_TCB *)0)
		{
			/* ɾ���ڵ� ��Ϊ�ǵ�һ��������ȫ����ʼ����0���� */
			p_rdy_list->NbrEnries = (OS_OBJ_QTY)0;
			p_rdy_list->HeadPtr   = (OS_TCB *)0;
			p_rdy_list->TailPtr   = (OS_TCB *)0;
			
			
		}
		else /* ������ֹһ���ڵ� */
		{
			p_rdy_list->NbrEnries--;
			p_tcb2->PrevPtr     = (OS_TCB *)0;
			p_rdy_list->HeadPtr = p_tcb2;
		}
		OS_PrioRemove(p_tcb->prio);  //�����б����Ƴ�
	}
	else   /* �������в�ֹһ���ڵ� */   
	{
		p_rdy_list->NbrEnries--;
		p_tcb2->NextPtr = p_tcb1; /* ǰһ���ڵ�ָ���һ���ڵ� */
		
		/* ���ɾ������β��� */
		if(p_tcb1 == (OS_TCB *)0) 
		{
			p_rdy_list->TailPtr = p_tcb2;
		}
		else  /* ����β��� */
		{
			p_tcb1->PrevPtr = p_tcb2;
		}
		
		OS_PrioRemove(p_tcb->prio);  //�����б����Ƴ�
		/* ɾ���Ľڵ�Ͽ� */
		p_tcb->NextPtr = (OS_TCB *)0;
		p_tcb->PrevPtr = (OS_TCB *)0;
		
	}
	
}
