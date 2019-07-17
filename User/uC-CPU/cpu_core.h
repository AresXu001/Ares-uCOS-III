#ifndef  CPU_CORE_MODULE_PRESENT
#define  CPU_CORE_MODULE_PRESENT

#ifdef CPU_CORE_MODULE   
#define CPU_CORE_EXT
#else
#define CPU_CORE_EXT  extern
#endif

#include "cpu.h"
#include "cpu_cfg.h"
#include "lib_def.h"

#if ((CPU_CFG_TS_32_EN == DEF_ENABLE) || \
		(CPU_CFG_TS_64_EN == DEF_ENABLE))
#define CPU_CFG_TS_EN      DEF_ENABLE
#else
#define CPU_CFG_TS_EN      DEF_DISABLE  
#endif

#if ((CPU_CFG_TS_EN == DEF_ENABLE) || \
		(defined(CPU_CFG_INT_DIS_MEAS_EN)))
#define CPU_CFG_TS_TMR_EN     DEF_ENABLE
#else   
#define CPU_CFG_TS_TMR_EN     DEF_DISABLE
#endif


/*
*******************************************************************************************
*                           ʱ����������Ͷ���
*                           ��cpu_core.h�ļ�����
***************************************************************************************
*/
typedef CPU_INT32U    CPU_TS32;
typedef CPU_INT32U    CPU_TS_TMR_FREQ;
typedef CPU_TS32      CPU_TS;
typedef CPU_INT32U    CPU_TS_TMR;


/*
*******************************************************************************************
*                           ȫ�ֱ�������
*                           ��cpu_core.h�ļ�����
*******************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLE)	//ʱ���ʹ��
CPU_CORE_EXT   CPU_TS_TMR_FREQ  CPU_TS_TmrFreq_Hz;
#endif  /*��CPU_TS_TmrFreq_Hz ��һ���� cpu_core.h �ж����ȫ�ֱ�����
��ʾ  CPU  ��ϵͳʱ�ӣ������С��Ӳ����أ����ʹ��  STM32F103  ϵ�У��Ǿ͵���
72000000HZ��*/

/*
********************************************************************************************
*                          ��������
********************************************************************************************
*/
/* CPU ��ʼ������ */
void CPU_Init(void);


#if ((CPU_CFG_TS_EN == DEF_ENABLE) || \
		(CPU_CFG_TS_TMR_EN == DEF_ENABLE))
static void CPU_TS_Init(void);
#endif


#if (CPU_CFG_TS_TMR_EN == DEF_ENABLE)
void  CPU_TS_TmrInit(void);
#endif


#if (CPU_CFG_TS_TMR_EN == DEF_ENABLE)
void  CPU_TS_TmrFreqSet (CPU_TS_TMR_FREQ  freq_hz);
#endif


#if (CPU_CFG_TS_TMR_EN == DEF_ENABLE)
CPU_TS_TMR  CPU_TS_TmrRd(void);
#endif

/*
*********************************************************************************************************
*                                          ��������
*                                    �ڲ�Ʒ��BSP�ļ����涨��
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq(void);

#endif

