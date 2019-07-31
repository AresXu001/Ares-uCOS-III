#include "os.h"

/* ��ʼ��TCBΪĬ��ֵ0 */
void OS_TaskInitTCB(OS_TCB *p_tcb)
{
	p_tcb->StkPtr         = (CPU_STK    * )0;
	p_tcb->StkSize        = (CPU_STK_SIZE )0u;
	
	p_tcb->TaskDelayTicks = (OS_TICK      )0u;
	
	p_tcb->prio           = (OS_PRIO      )OS_PRIO_INIT;
	
	p_tcb->NextPtr        = (OS_TCB      *)0;
	p_tcb->PrevPtr        = (OS_TCB      *)0;
}

void OSTaskCreate(OS_TCB       *p_tcb,        // ������ƿ�
									OS_TASK_PTR  p_task,       // ������ں���
									void         *p_arg,       // ������ں����β�
									OS_PRIO      prio,         // �������ȼ�
									CPU_STK      *p_stk_base,  //�����ջ����ַ
									CPU_STK_SIZE stk_size,     //�����ջ��С
									OS_TICK     time_quata,    //����ʱ��Ƭ
									OS_ERR       *p_err)       //�������ʶ����
{
	CPU_STK   *p_sp;
	CPU_SR_ALLOC();
	
	/* ��ʼ��TCBΪĬ��ֵ  */
	OS_TaskInitTCB(p_tcb);
	
	p_sp = OSTaskStkInit(p_task,
											 p_arg,
											 p_stk_base,
											 stk_size);
	
	p_tcb->prio = prio;        //�����������ȼ�
	p_tcb->StkPtr   = p_sp;    //���������ջջ����ַ
	p_tcb->StkSize  = stk_size;//���������ջ��С
	
	/* ʱ��Ƭ��س�ʼ�� */
	p_tcb->TimeQuanta = time_quata;

#if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
	p_tcb->TimeQuantaCtr = time_quata;
#endif
	
	 /* �����ٽ�� */ 
	OS_CRITICAL_ENTER(); 
	
	/* ��������ӵ���������б� */
	OS_PrioInsert(p_tcb->prio);
	OS_RdyListInsertTail(p_tcb);
	
	/* �˳��ٽ��� */
	OS_CRITICAL_EXIT();
	
	*p_err  = OS_ERR_NONE;
}

#if OS_CFG_TASK_SUSPEND_EN   > 0u
void OS_TaskSuspen(OS_TCB *p_tcb,OS_ERR *p_err)
{
	CPU_SR_ALLOC();

#if 0   /* ���ο�ʼ */
	
#ifdef OS_SAFETY_CRITICAL
	if(p_err == (OS_ERR *)0)
	{
		OS_SAFETY_CRITICAL_EXCEPTION();
		return;
	}
#endif

#if OS_CFG_CALLED_FROM_ISR_CHK_EN  > 0u
  /* ������ISR������øú��� */
	if(OSIntNestingCtr > (OS_NESTING_CTR)0)
	{
		*p_err = OS_ERR_TASK_SUSPEND_ISR;
		return;
	}
#endif	
	
	/* ���ܹ���������� */
	if(p_tcb == &OSIdleTaskTCB)
	{
		*p_err = OS_ERR_TASK_SUSPEND_IDLE;
		return;
	}
	
#if OS_CFG_ISR_POST_DEFERRED_EN  > 0u
	/* ���ܹ����жϴ������� */
	if(p_tcb == &OSIntQTaskTCB)
	{
		*p_err = OS_ERR_TASK_SUSPEND_INT_HANDLER;
		return;
	}
#endif
	
#endif  /* ���ν��� */
	
	CPU_CRITICAL_ENTER();
	
	/* �Ƿ�����Լ� */
	if(p_tcb == (OS_TCB *)0)
	{
		p_tcb = OSTCBCurPtr;
	}
	
	if(p_tcb == OSTCBCurPtr)
	{
		/* �����������ס���ܹ����Լ� */
		if(OSSchedLockNestingCtr > (OS_NESTING_CTR)0)
		{
			CPU_CRITICAL_EXIT();
			*p_err = OS_ERR_SCHED_LOCKED;
			return;
		}
	}
	
	*p_err = OS_ERR_NONE;
	
	/* ���������״̬��������Ķ��� */
	switch(p_tcb->TaskState)
	{
		case OS_TASK_STATE_RDY:
			OS_CRITICAL_ENTER_CPU_CRITICAL_EXIT();
			p_tcb->TaskState = OS_TASK_STATE_SUSPENDED;
			p_tcb->SuspenCtr = (OS_NESTING_CTR)1;
			OS_RdyListRemove(p_tcb);
			OS_CRITICAL_EXIT_NO_SCHED();
			break;
		case OS_TASK_STATE_DLY:
			p_tcb->TaskState  = OS_TASK_STATE_DLY_SUSPENDED;
			p_tcb->SuspenCtr = (OS_NESTING_CTR)1;
			CPU_CRITICAL_EXIT();
			break;
		case OS_TASK_STATE_PEND:
			p_tcb->TaskState = OS_TASK_STATE_PEND_SUSPENDED;
			p_tcb->SuspenCtr = (OS_NESTING_CTR)1;
			CPU_CRITICAL_EXIT();
			break;
		case OS_TASK_STATE_PEND_TIMEOUT:
			p_tcb->TaskState = OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED;
			p_tcb->SuspenCtr = (OS_NESTING_CTR)1;
			CPU_CRITICAL_EXIT();
			break;
		case OS_TASK_STATE_SUSPENDED:
		case OS_TASK_STATE_DLY_SUSPENDED:
		case OS_TASK_STATE_PEND_SUSPENDED:
		case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
			p_tcb->SuspenCtr++;
			CPU_CRITICAL_EXIT();
			break;
		default:
			CPU_CRITICAL_EXIT();
			*p_err = OS_ERR_STATE_INVALID;
			return;
	}
	
	/* �����л�*/
	OSSched();
}

#endif

#if OS_CFG_TASK_SUSPEND_EN   > 0u
void OSTaskResume(OS_TCB *p_tcb,
						OS_ERR *p_err)
{
	CPU_SR_ALLOC();
	
#if 0   /* ���ο�ʼ */
	
#ifdef OS_SAFETY_CRITICAL
	 if(p_err == (OS_ERR *)0)
	 {
		OS_SAFETY_CRITICAL_EXCEPTION();
		return;
	 }
#endif
	 
#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
	 /* ������ISR�����е��øú��� */
	 if(OSIntNestingCtr > (OS_NEST_CTR)0)
	 {
		 *p_err = OS_ERR_TASK_RESUME_ISR;
		 return;
	 }
#endif
	 
	 CPU_CRITICAL_ENTER();
#if OS_CFG_ARG_CHK_EN  > 0u
	 /* ���ָܻ��Լ� */
	 if((p_tcb == (OS_TCB *)0) || 
		 (p_tcb == OSTCBCurPtr))
	 {
		 CPU_CRITICAL_EXIT();
		 *p_err = OS_ERR_TASK_RESUME_SELF;
		 return;
	 }
#endif

#endif  /*���ν��� */
	 
	 *p_err = OS_ERR_NONE;
	 /* ���������״̬�������ָ��Ķ��� */
	 switch(p_tcb->TaskState)
	 {
		case OS_TASK_STATE_RDY:
		case OS_TASK_STATE_DLY:
		case OS_TASK_STATE_PEND:
		case OS_TASK_STATE_PEND_TIMEOUT:
			CPU_CRITICAL_EXIT();
			*p_err = OS_ERR_TASK_NOT_SUSPENDED;
			break;
		
		case OS_TASK_STATE_SUSPENDED:
			OS_CRITICAL_ENTER_CPU_CRITICAL_EXIT();
			p_tcb->SuspenCtr--;
			if(p_tcb->SuspenCtr == (OS_NESTING_CTR)0)
			{
				p_tcb->TaskState = OS_TASK_STATE_RDY;
				OS_TaskRdy(p_tcb);
			}
			OS_CRITICAL_EXIT_NO_SCHED();
			break;
			
		case OS_TASK_STATE_DLY_SUSPENDED:
			p_tcb->SuspenCtr--;
			if(p_tcb->SuspenCtr == (OS_NESTING_CTR)0)
			{
				p_tcb->TaskState = OS_TASK_STATE_DLY;
			}
			CPU_CRITICAL_EXIT();
			break;
			
		case OS_TASK_STATE_PEND_SUSPENDED:
			p_tcb->SuspenCtr--;
			if(p_tcb->SuspenCtr == (OS_NESTING_CTR)0)
			{
				p_tcb->TaskState = OS_TASK_STATE_PEND;
			}
			CPU_CRITICAL_EXIT();
			break;
			
		case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
		   p_tcb->SuspenCtr--;
			if(p_tcb->SuspenCtr == (OS_NESTING_CTR)0)
			{
				p_tcb->TaskState = OS_TASK_STATE_PEND_TIMEOUT;
			}
			CPU_CRITICAL_EXIT();
			break;
			
		default:
			CPU_CRITICAL_EXIT();
			*p_err = OS_ERR_STATE_INVALID;
			return;
	 }
	 
	 /* �����л� */
	 OSSched();
}
#endif

#if OS_CFG_TASK_DEL_EN  > 0u

void OSTaskDel(OS_TCB *p_tcb,
			      OS_ERR *p_err)
{
	CPU_SR_ALLOC();
	
	/* �������� */
	if(p_tcb == &OSIdleTaskTCB)
	{
		*p_err = OS_ERR_TASK_DEL_IDLE;
		return;
	}
}

#endif
