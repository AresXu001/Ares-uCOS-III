
;********************************************************************************************************
;                       PUBLIC FUNCTIONS������Ϊ�ⲿ�ɵ���
;********************************************************************************************************

        EXPORT  CPU_IntDis
        EXPORT  CPU_IntEn
		EXPORT  CPU_CntLeadZeros
		EXPORT  CPU_CntTrialZeros



;********************************************************************************************************
;                                      �������ָ��
;********************************************************************************************************

        AREA |.text|, CODE, READONLY, ALIGN=2
        THUMB
        REQUIRE8
        PRESERVE8
			

;********************************************************************************************************
;                                    ʧ�� and ʹ�� �ж�
;
; Description : Disable/Enable interrupts.
;
; Prototypes  : void  CPU_IntDis(void);
;               void  CPU_IntEn (void);
;********************************************************************************************************

CPU_IntDis
        CPSID   I
        BX      LR


CPU_IntEn
        CPSIE   I
        BX      LR



;********************************************************************************************************
;                                      CRITICAL SECTION FUNCTIONS
;
; Description : Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking, the
;               state of the interrupt disable flag is stored in the local variable 'cpu_sr' & interrupts
;               are then disabled ('cpu_sr' is allocated in all functions that need to disable interrupts).
;               The previous interrupt state is restored by copying 'cpu_sr' into the CPU's status register.
;
; Prototypes  : CPU_SR  CPU_SR_Save   (void);
;               void    CPU_SR_Restore(CPU_SR  cpu_sr);
;
; Note(s)     : (1) These functions are used in general like this :
;
;                       void  Task (void  *p_arg)
;                       {
;                           CPU_SR_ALLOC();                     /* Allocate storage for CPU status register */
;                               :
;                               :
;                           CPU_CRITICAL_ENTER();               /* cpu_sr = CPU_SR_Save();                  */
;                               :
;                               :
;                           CPU_CRITICAL_EXIT();                /* CPU_SR_Restore(cpu_sr);                  */
;                               :
;                       }
;********************************************************************************************************
CPU_SR_Save     ;�����ٽ���
	MRS    R0,PRIMASK
	CPSID  I
	BX    LR
	 
CPU_SR_Restore  ;�˳��ٽ���
	MSR    PRIMASK, R0
	BX     LR
	
;******************************************************************************
;*  
;*                   ����ǰ����
;*                 ����������CPU_DATA CPU_CntLeadZeros(CPU_DATA val)
;*******************************************************************************
CPU_CntLeadZeros
	CLZ R0,R0
	BX  LR

;************************************************************************************
;*                               �������
;*                     ��������������������CPU_DATA CPU_CntTrialZeros(CPU_DATA val)
;************************************************************************************
CPU_CntTrialZeros
	RBIT  R0,R0
	CLZ   R0,R0
	BX    LR
	
	END
			
			
	
			
			