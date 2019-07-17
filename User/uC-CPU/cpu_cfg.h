#ifndef  CPU_CFG_MODULE_PRESENT
#define  CPU_CFG_MODULE_PRESENT

#include <cpu_def.h>
#include <cpu_core.h>
#include <os_cpu.h>


#define CPU_CFG_TS_32_EN     DEF_ENABLE
#define CPU_CFG_TS_64_EN     DEF_ENABLE

#define CPU_CFG_TS_TMR_SIZE   CPU_WORD_SIZE_32

/* 测量关中断时间配置宏 */
#define COU_CFG_INT_DIS_MEAS_EN       0u       

/*
***************************************************************************************
*                         时间戳配置
***************************************************************************************
*/


#if  OS_CFG_TS_EN  ==   1u
#define OS_TS_GET()    (CPU_TS)CPU_TS_TmrRd()
#else
#define OS_TS_GET()     (CPU_TS)0u
#endif



#endif

