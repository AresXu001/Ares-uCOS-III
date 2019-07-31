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


