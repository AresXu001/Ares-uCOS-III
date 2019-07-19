#include "os.h"

/* ���ȼ��� */
CPU_DATA  OSPrioTbl[OS_PRIO_TBL_SIZE];  


/* OS_PrioInit()�������ڳ�ʼ�����ȼ���
��  OSInit()�����б�����*/
void OS_PrioInit(void)
{
	CPU_DATA i;
	
	for(i = 0u;i < OS_PRIO_TBL_SIZE; i++)
	{
		OSPrioTbl[i] = (CPU_DATA)0;
	}
}

/*
OS_PrioInsert()����������λ���ȼ�������Ӧ��λ��
�ᱻ  OSTaskCreate()��������
*/
void OS_PrioInsert(OS_PRIO prio)
{
	CPU_DATA bit;
	CPU_DATA bit_nbr;
	OS_PRIO  ix;
	
	ix = prio  /   DEF_INT_CPU_NBR_BITS;
	
	/*��������������ȼ�������  DEF_INT_CPU_NBR_BITS ֮��*/
	bit_nbr = (CPU_DATA)prio & (DEF_INT_CPU_NBR_BITS);
	
	 /* ��ȡ���ȼ������ȼ����ж�Ӧ��λ��λ�� */ 
	bit = 1u;
	bit <<= (DEF_INT_CPU_NBR_BITS - 1u) - bit_nbr;
	
	 /* �����ȼ������ȼ����ж�Ӧ��λ�� 1 */ 
	OSPrioTbl[ix] |= bit;
}

/*
OS_PrioRemove()��������������ȼ�������Ӧ��λ
*/
void OS_PrioRemove(OS_PRIO prio)
{
	CPU_DATA bit;
	CPU_DATA bit_nbr;
	OS_PRIO  ix;
	
	ix = prio  /   DEF_INT_CPU_NBR_BITS;
	
	/*��������������ȼ�������  DEF_INT_CPU_NBR_BITS ֮��*/
	bit_nbr = (CPU_DATA)prio & (DEF_INT_CPU_NBR_BITS);
	
	/* ��ȡ���ȼ������ȼ����ж�Ӧ��λ��λ�� */ 
	bit = 1u;
	bit <<= (DEF_INT_CPU_NBR_BITS - 1u) - bit_nbr;
	 
	/* �����ȼ������ȼ����ж�Ӧ��λ�� 0 */ 
	OSPrioTbl[ix] &= ~bit;
}

/* �������ڴ����ȼ����в�����ߵ����ȼ� */
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
	
	/* CPU_CntLeadZeros����ǰ���� */
	prio += (OS_PRIO)CPU_CntLeadZeros(*p_tbl);
	
	return prio;
}

