#include "os.h"

/* 初始化TCB为默认值0 */
void OS_TaskInitTCB(OS_TCB *p_tcb)
{
	p_tcb->StkPtr         = (CPU_STK    * )0;
	p_tcb->StkSize        = (CPU_STK_SIZE )0u;
	
	p_tcb->TaskDelayTicks = (OS_TICK      )0u;
	
	p_tcb->prio           = (OS_PRIO      )OS_PRIO_INIT;
	
	p_tcb->NextPtr        = (OS_TCB      *)0;
	p_tcb->PrevPtr        = (OS_TCB      *)0;
}

void OSTaskCreate(OS_TCB       *p_tcb,        // 任务控制块
									OS_TASK_PTR  p_task,       // 任务入口函数
									void         *p_arg,       // 任务入口函数形参
									OS_PRIO      prio,         // 任务优先级
									CPU_STK      *p_stk_base,  //任务堆栈基地址
									CPU_STK_SIZE stk_size,     //任务堆栈大小
									OS_TICK     time_quata,    //任务时间片
									OS_ERR       *p_err)       //任务故障识别码
{
	CPU_STK   *p_sp;
	CPU_SR_ALLOC();
	
	/* 初始化TCB为默认值  */
	OS_TaskInitTCB(p_tcb);
	
	p_sp = OSTaskStkInit(p_task,
											 p_arg,
											 p_stk_base,
											 stk_size);
	
	p_tcb->prio = prio;        //设置任务优先级
	p_tcb->StkPtr   = p_sp;    //保存任务堆栈栈顶地址
	p_tcb->StkSize  = stk_size;//保存任务堆栈大小
	
	/* 时间片相关初始化 */
	p_tcb->TimeQuanta = time_quata;

#if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
	p_tcb->TimeQuantaCtr = time_quata;
#endif
	
	 /* 进入临界段 */ 
	OS_CRITICAL_ENTER(); 
	
	/* 将任务添加到任务就绪列表 */
	OS_PrioInsert(p_tcb->prio);
	OS_RdyListInsertTail(p_tcb);
	
	/* 退出临界区 */
	OS_CRITICAL_EXIT();
	
	*p_err  = OS_ERR_NONE;
}


