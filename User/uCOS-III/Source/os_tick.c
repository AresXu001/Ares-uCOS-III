#include "os.h"

/* 初始化时基列表的数据域*/
void OS_TickListInit(void)
{
	OS_TICK_SPOKE_IX   i;
	OS_TICK_SPOKE      *p_spoke;
	
	for(i = 0u;i < OSCfg_TickWheelSize;i++)
	{
		p_spoke                = (OS_TICK_SPOKE *)&OSCfg_TickWheel[i];
		p_spoke->FirstPtr      = (OS_TCB *)0;
		p_spoke->NbrEntries    = (OS_OBJ_QTY)0u;
		p_spoke->NbrEntriesMax = (OS_OBJ_QTY)0u;
	}
}

/* 插入时基列表 */
void OS_TickListInsert(OS_TCB *p_tcb,OS_TICK time)
{
	OS_TICK_SPOKE_IX spoke;
	OS_TICK_SPOKE    *p_spoke;
	OS_TCB           *p_tcb0;
	OS_TCB           *p_tcb1;
	
	p_tcb->TickCtrMatch  = OSTickCtr + time;
	p_tcb->TickRemain    = time;
	
	spoke = (OS_TICK_SPOKE_IX)(p_tcb->TickCtrMatch % OSCfg_TickWheelSize);
	p_spoke = &OSCfg_TickWheel[spoke];
	
	/* 插入到OSCfg_TickWheel[spoke] 的第一个节点 */
	if(p_spoke->NbrEntries == (OS_OBJ_QTY)0u)
	{
		p_tcb->TickNextPtr     = (OS_TCB *)0;
		p_tcb->TickPrevPtr     = (OS_TCB *)0;
		p_spoke->FirstPtr      =  p_tcb;
		p_spoke->NbrEntriesMax =  1u;
	}
	else/* 如果插入的不是第一个节点，则按照TickRemain大小升序排列 */
	{
		/* 获取第一个节点指针 */
		p_tcb1 = p_spoke->FirstPtr;
		while(p_tcb1 != (OS_TCB *)0)
		{
			/* 计算比较剩余时间 */
			p_tcb1->TickRemain = p_tcb1->TickCtrMatch - OSTickCtr;
			
			/* 插入到比较节点的后面 */
			if(p_tcb->TickRemain > p_tcb1->TickRemain)
			{
				if(p_tcb1->TickNextPtr != (OS_TCB *)0)
				{
					/* 寻找下一比较节点 */
					p_tcb1 =  p_tcb1->TickNextPtr;
				}
				else  /* 在最后一个节点插入 */
				{
					p_tcb->TickNextPtr  = (OS_TCB *)0;
					p_tcb->TickPrevPtr  = p_tcb1;
					p_tcb1->TickNextPtr = p_tcb;
					p_tcb1              = (OS_TCB *)0;
				}
			}
			else /* 插入到比较节点的前面 */
			{
				/* 在第一个节点插入 */
				if(p_tcb1->TickPrevPtr == (OS_TCB *)0)
				{
					p_tcb->TickPrevPtr = (OS_TCB *)0;
					p_tcb->TickNextPtr = p_tcb1;
					p_tcb1->TickPrevPtr = p_tcb;
					p_spoke->FirstPtr   = p_tcb;
				}
				else
				{
					/* 插入到两个节点之间 */
					p_tcb0   = p_tcb1->TickPrevPtr;
					p_tcb->TickPrevPtr = p_tcb0;
					p_tcb->TickNextPtr = p_tcb1;
					p_tcb0->TickNextPtr = p_tcb;
					p_tcb1->TickPrevPtr = p_tcb;
				}
				/* 跳出while循环 */
				p_tcb1 = (OS_TCB *)0;
			}
		}
		
		/* 节点成功插入 */
		p_spoke->NbrEntries++;
	}
	
	/* 刷新NbrEntriesMax的值 */
	if(p_spoke->NbrEntriesMax < p_spoke->NbrEntries)
	{
		p_spoke->NbrEntriesMax = p_spoke->NbrEntries;
	}
	
	/* 任务TCB中的TickSpokeptr回指根节点 */
	p_tcb->TickSpokePtr  = p_spoke;	
}

/* 从时基列表中移除一个任务 */
void OS_TickListRemove(OS_TCB *p_tcb)
{
	OS_TICK_SPOKE *p_spoke;
	OS_TCB        *p_tcb1;
	OS_TCB        *p_tcb2;
	
	/* 获取任务TCB所在链表的根指针 */
	p_spoke  = p_tcb->TickSpokePtr;
	
	/* 确保任务在链表中 */
	if(p_spoke != (OS_TICK_SPOKE *)0)
	{
		p_tcb->TickRemain =  (OS_TICK)0u;
		
		/* 如果移除的刚好是第一个节点 */
		if(p_spoke->FirstPtr == p_tcb)
		{
			/* 更新第一个节点，原来的节点需要被移除 */
			p_tcb1            = (OS_TCB *)p_tcb->TickNextPtr;
			p_spoke->FirstPtr = p_tcb1;
			if(p_tcb1 != (OS_TCB *)0)
			{
				p_tcb1->TickPrevPtr = (OS_TCB *)0;
			}
		}
		else  /* 要移除的不是第一个节点 */
		{
			/* 保存要移除的节点的前后节点的指针 */
			p_tcb1  = p_tcb->TickPrevPtr;
			p_tcb2  = p_tcb->TickNextPtr;
			
			/* 节点移除，将节点前后的两个节点连接在一起 */
			p_tcb1->TickNextPtr  = p_tcb2;
			if(p_tcb2 != (OS_TCB *)0)
			{
				p_tcb2->TickPrevPtr  = p_tcb1;
			}
		}
		
	   /* 复位任务TCB中时基列表相关的字段成员 */
		p_tcb->TickNextPtr  = (OS_TCB        *)0;
		p_tcb->TickPrevPtr  = (OS_TCB        *)0;
		p_tcb->TickSpokePtr = (OS_TICK_SPOKE *)0;
		p_tcb->TickCtrMatch = (OS_TICK        )0u;
		
		/* 节点减1 */
		p_spoke->NbrEntries--;
	}
}

/* 时基列表更新 由OSTimeTick()函数调用 */
void OS_TickListUpdate(void)
{
	OS_TICK_SPOKE_IX  spoke;
	OS_TICK_SPOKE     *p_spoke;
	OS_TCB            *p_tcb;
	OS_TCB  				*p_tcb_next;
	CPU_BOOLEAN       done;
	
	CPU_SR_ALLOC();
	
	/* 进入临界段 */
	OS_CRITICAL_ENTER();
	
	/* 时基计数器++ */
	OSTickCtr++;
	
	spoke   = (OS_TICK_SPOKE_IX)(OSTickCtr % OSCfg_TickWheelSize);
	p_spoke = &OSCfg_TickWheel[spoke];
	
	p_tcb  = p_spoke->FirstPtr;
	done   = DEF_FALSE;
	
	while(done == DEF_FALSE)
	{
		if(p_tcb != (OS_TCB *)0)
		{
			p_tcb_next = p_tcb->TickNextPtr;
			p_tcb->TickRemain  = p_tcb->TickCtrMatch - OSTickCtr;
			
			/* 任务延时时间到 */
			if(OSTickCtr == p_tcb->TickCtrMatch)
			{
				/* 让任务就绪 */
				OS_TaskRdy(p_tcb);
			}
			else
			{
				/* 如果第一个节点延时时间未到，则退出while循环
				   因为链表是根据升序排列的，第一个节点延时时间未到
				   则后面的延时全部都没到*/
				done = DEF_TRUE;
			}
			
			/* 如果第一个节点延时时间到，则继续遍历链表，看看还有没有其他
			  延时时间到的任务，有则使其就绪*/
			p_tcb = p_tcb_next;
		}
		else
		{
			done = DEF_TRUE;
		}
	}
	/* 退出临界区 */
	OS_CRITICAL_EXIT();
}


