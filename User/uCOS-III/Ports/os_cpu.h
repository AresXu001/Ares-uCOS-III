#ifndef  _OS_CPU_H_
#define  _OS_CPU_H_

/*
*********************************************************************************************************
*                                               宏定义
*********************************************************************************************************
*/

#ifndef  NVIC_INT_CTRL
#define  NVIC_INT_CTRL                      *((CPU_REG32 *)0xE000ED04)   /* 中断控制及状态寄存器 SCB_ICSR */
#endif

#ifndef  NVIC_PENDSVSET
#define  NVIC_PENDSVSET                                    0x10000000    /* 触发PendSV异常的值 Bit28：PENDSVSET */
#endif

#define  OS_TASK_SW()               NVIC_INT_CTRL = NVIC_PENDSVSET     //触发PendSV中断
#define  OSIntCtxSw()               NVIC_INT_CTRL = NVIC_PENDSVSET

#define OS_CFG_TS_EN    1u
/*
*********************************************************************************************************
*                                              函数声明
*********************************************************************************************************
*/
extern void OSStartHighRdy(void);
extern void PendSV_Handler(void);


#endif   /* OS_CPU_H */
