#include <os_cfg_app.h>
#include "os.h"


/* ������������ջ */
CPU_STK OSCfg_IdleTaskStk[OS_CFG_IDLE_TASK_SIZE];

/* ʱ���б� */
OS_TICK_SPOKE   OSCfg_TickWheel[OS_CFG_TICK_WHEEL_SIZE];
OS_OBJ_QTY  const OSCfg_TickWheelSize = (OS_OBJ_QTY)OS_CFG_TICK_WHEEL_SIZE;

/* ���������ջ��ʼ��ַ ��const˵���ǲ������޸ĵģ�����ϵͳ������ */
CPU_STK *const OSCfg_IdleTaskStkBasePtr = (CPU_STK *)&OSCfg_IdleTaskStk[0];

/* ���������ջ�Ĵ�С */
CPU_STK_SIZE const OSCfg_IdleTaskStkSize = (CPU_STK_SIZE)OS_CFG_IDLE_TASK_SIZE;





