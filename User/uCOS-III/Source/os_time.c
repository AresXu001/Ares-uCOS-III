#include "os.h"

void OSTimeTick(void)
{
	unsigned int i;
	
	for(i = 0;i < OS_CFG_PRIO_MAX;i++)
	{
		if(OSRdyList[i].HeadPtr->TaskDelayTicks > 0)
		{
			OSRdyList[i].HeadPtr->TaskDelayTicks--;
		}
	}
	OSSched();
}

void OSTimeDly(OS_TICK dly)
{
	/* 设置延时时间 */
	OSTCBCurPtr->TaskDelayTicks = dly;
	
	/* 进行任务调度 */
	OSSched();
}


