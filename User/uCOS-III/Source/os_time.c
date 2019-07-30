#include "os.h"

void OSTimeTick(void)
{
	unsigned int i;	
	CPU_SR_ALLOC();
	
	/* �����ٽ��� */
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
	
#if 0
	for(i = 0;i < OS_CFG_PRIO_MAX;i++)
	{
		if(OSRdyList[i].HeadPtr->TaskDelayTicks > 0)
		{
			OSRdyList[i].HeadPtr->TaskDelayTicks--;
			if(OSRdyList[i].HeadPtr->TaskDelayTicks == 0)
			{
				/* ��ʱʱ�䵽�����������*/
				OS_PrioInsert(i);
			}
		}
	}
#endif
	
	/* ����ʱ���б� */
	OS_TickListUpdate();
	/* �˳��ٽ��� */
	OS_CRITICAL_ENTER();
	
	/* ������� */
	OSSched();
}

void OSTimeDly(OS_TICK dly)
{
#if 0
	/* ������ʱʱ�� */
	OSTCBCurPtr->TaskDelayTicks = dly;
	
	/* ����������� */
	OSSched();
#endif
	
	CPU_SR_ALLOC();
	
	/* �����ٽ��� */
	OS_CRITICAL_ENTER();
#if 0	
	OSTCBCurPtr->TaskDelayTicks = dly;
	
	/* �Ӿ����б���ɾ�� */
	OS_PrioRemove(OSTCBCurPtr->prio);
#endif
	
	/* ����ʱ���б� */
	OS_TickListInsert(OSTCBCurPtr,dly);
	
	/* �Ӿ����б����Ƴ� */
	OS_RdyListRemove(OSTCBCurPtr);
	
	/* �˳��ٽ��� */
	OS_CRITICAL_EXIT();
	
	/* ����������� */
	OSSched();
		
}


