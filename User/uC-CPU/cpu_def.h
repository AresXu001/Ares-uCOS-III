#ifndef  CPU_DEF_MODULE_PRESENT
#define  CPU_DEF_MODULE_PRESENT

#include <cpu.h>

                                                        /* ---------------------- CPU WORD SIZE ----------------------- */
#define  CPU_WORD_SIZE_08                          1u   /*  8-bit word size (in octets).                                */
#define  CPU_WORD_SIZE_16                          2u   /* 16-bit word size (in octets).                                */
#define  CPU_WORD_SIZE_32                          4u   /* 32-bit word size (in octets).                                */
#define  CPU_WORD_SIZE_64                          8u   /* 64-bit word size (in octets).                                */


/*
*********************************************************************************************************
*                                          �ٽ������
*********************************************************************************************************
*/  
#define  CPU_CRITICAL_METHOD_NONE                  0u  
#define  CPU_CRITICAL_METHOD_INT_DIS_EN            1u   /* DIS/EN       �ж�*/
#define  CPU_CRITICAL_METHOD_STATUS_STK            2u   /* Push/Pop     �ж�״̬�������ջ */
#define  CPU_CRITICAL_METHOD_STATUS_LOCAL          3u   /* Save/Restore �ж�״̬���ֲ�����*/

/* һ���ֽڳ���Ϊ8λ */																
#define  DEF_OCTET_NBR_BITS                        8u
/* CPU������λ���� */
#define  DEF_INT_CPU_NBR_BITS        (CPU_CFG_DATA_SIZE * DEF_OCTET_NBR_BITS)

#endif /* CPU_DEF_MODULE_PRESENT */
