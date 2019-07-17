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
*                           时间戳数据类型定义
*                           在cpu_core.h文件定义
***************************************************************************************
*/
typedef CPU_INT32U    CPU_TS32;
typedef CPU_INT32U    CPU_TS_TMR_FREQ;
typedef CPU_TS32      CPU_TS;
typedef CPU_INT32U    CPU_TS_TMR;


/*
*******************************************************************************************
*                           全局变量定义
*                           在cpu_core.h文件定义
*******************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLE)	//时间戳使能
CPU_CORE_EXT   CPU_TS_TMR_FREQ  CPU_TS_TmrFreq_Hz;
#endif  /*：CPU_TS_TmrFreq_Hz 是一个在 cpu_core.h 中定义的全局变量，
表示  CPU  的系统时钟，具体大小跟硬件相关，如果使用  STM32F103  系列，那就等于
72000000HZ。*/

/*
********************************************************************************************
*                          函数声明
********************************************************************************************
*/
/* CPU 初始化函数 */
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
*                                          函数声明
*                                    在产品的BSP文件里面定义
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq(void);

#endif

