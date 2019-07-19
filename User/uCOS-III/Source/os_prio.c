#include "os.h"

/* 优先级表 */
CPU_DATA  OSPrioTbl[OS_PRIO_TBL_SIZE];  


/* OS_PrioInit()函数用于初始化优先级表，
在  OSInit()函数中被调用*/
void OS_PrioInit(void)
{
	CPU_DATA i;
	
	for(i = 0u;i < OS_PRIO_TBL_SIZE; i++)
	{
		OSPrioTbl[i] = (CPU_DATA)0;
	}
}

/*
OS_PrioInsert()函数用于置位优先级表中相应的位，
会被  OSTaskCreate()函数调用
*/
void OS_PrioInsert(OS_PRIO prio)
{
	CPU_DATA bit;
	CPU_DATA bit_nbr;
	OS_PRIO  ix;
	
	ix = prio  /   DEF_INT_CPU_NBR_BITS;
	
	/*求余操作，将优先级限制在  DEF_INT_CPU_NBR_BITS 之内*/
	bit_nbr = (CPU_DATA)prio & (DEF_INT_CPU_NBR_BITS);
	
	 /* 获取优先级在优先级表中对应的位的位置 */ 
	bit = 1u;
	bit <<= (DEF_INT_CPU_NBR_BITS - 1u) - bit_nbr;
	
	 /* 将优先级在优先级表中对应的位置 1 */ 
	OSPrioTbl[ix] |= bit;
}

/*
OS_PrioRemove()函数用于清除优先级表中相应的位
*/
void OS_PrioRemove(OS_PRIO prio)
{
	CPU_DATA bit;
	CPU_DATA bit_nbr;
	OS_PRIO  ix;
	
	ix = prio  /   DEF_INT_CPU_NBR_BITS;
	
	/*求余操作，将优先级限制在  DEF_INT_CPU_NBR_BITS 之内*/
	bit_nbr = (CPU_DATA)prio & (DEF_INT_CPU_NBR_BITS);
	
	/* 获取优先级在优先级表中对应的位的位置 */ 
	bit = 1u;
	bit <<= (DEF_INT_CPU_NBR_BITS - 1u) - bit_nbr;
	 
	/* 将优先级在优先级表中对应的位清 0 */ 
	OSPrioTbl[ix] &= ~bit;
}

/* 函数用于从优先级表中查找最高的优先级 */
OS_PRIO OS_PrioGetHighest(void)
{
	CPU_DATA *p_tbl;
	OS_PRIO   prio;
	
	prio = (OS_PRIO)0;
	
	p_tbl = &OSPrioTbl[0];
	
	while(*p_tbl == (CPU_DATA)0)
	{
		prio += DEF_INT_CPU_NBR_BITS;
		p_tbl++;
	}
	
	/* CPU_CntLeadZeros计算前导零 */
	prio += (OS_PRIO)CPU_CntLeadZeros(*p_tbl);
	
	return prio;
}

