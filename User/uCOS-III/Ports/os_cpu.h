#ifndef  _OS_CPU_H_
#define  _OS_CPU_H_

/*
*********************************************************************************************************
*                                               �궨��
*********************************************************************************************************
*/

#ifndef  NVIC_INT_CTRL
#define  NVIC_INT_CTRL                      *((CPU_REG32 *)0xE000ED04)   /* �жϿ��Ƽ�״̬�Ĵ��� SCB_ICSR */
#endif

#ifndef  NVIC_PENDSVSET
#define  NVIC_PENDSVSET                                    0x10000000    /* ����PendSV�쳣��ֵ Bit28��PENDSVSET */
#endif

#define  OS_TASK_SW()               NVIC_INT_CTRL = NVIC_PENDSVSET     //����PendSV�ж�
#define  OSIntCtxSw()               NVIC_INT_CTRL = NVIC_PENDSVSET

#define OS_CFG_TS_EN    1u
/*
*********************************************************************************************************
*                                              ��������
*********************************************************************************************************
*/
extern void OSStartHighRdy(void);
extern void PendSV_Handler(void);


#endif   /* OS_CPU_H */
