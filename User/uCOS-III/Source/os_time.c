#include "os.h"

void OSTimeTick(void)
{
	unsigned int i;	
	CPU_SR_ALLOC();
	
	/* 进入临界区 */
	OS_CRITICAL_ENTER();
	
#if 0
	for(i = 0;i < OS_CFG_PRIO_MAX;i++)
	{
		if(OSRdyList[i].HeadPtr->TaskDelayTicks > 0)
		{
			OSRdyList[i].HeadPtr->TaskDelayTicks--;
		}
	}	
#endif
	
	for(i = 0;i < OS_CFG_PRIO_MAX;i++)
	{
		if(OSRdyList[i].HeadPtr->TaskDelayTicks > 0)
		{
			OSRdyList[i].HeadPtr->TaskDelayTicks--;
			if(OSRdyList[i].HeadPtr->TaskDelayTicks == 0)
			{
				/* 延时时间到，让任务就绪*/
				OS_PrioInsert(i);
			}
		}
	}
	
	/* 退出临界区 */
	OS_CRITICAL_ENTER();
	
	/* 任务调度 */
	OSSched();
}

void OSTimeDly(OS_TICK dly)
{
#if 0
	/* 设置延时时间 */
	OSTCBCurPtr->TaskDelayTicks = dly;
	
	/* 进行任务调度 */
	OSSched();
#endif
	
	CPU_SR_ALLOC();
	
	/* 进入临界区 */
	OS_CRITICAL_ENTER();
	
	OSTCBCurPtr->TaskDelayTicks = dly;
	
	/* 从就绪列表中删除 */
	OS_PrioRemove(OSTCBCurPtr->prio);
	
	/* 退出临界区 */
	OS_CRITICAL_EXIT();
	
	/* 进行任务调度 */
	OSSched();
		
}


