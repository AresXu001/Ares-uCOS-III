#include <os_cfg_app.h>
#include "os.h"


/* 定义空闲任务堆栈 */
CPU_STK OSCfg_IdleTaskStk[OS_CFG_IDLE_TASK_SIZE];

/* 空闲任务堆栈起始地址 加const说明是不可以修改的，属于系统级任务 */
CPU_STK *const OSCfg_IdleTaskStkBasePtr = (CPU_STK *)&OSCfg_IdleTaskStk[0];

/* 空闲任务堆栈的大小 */
CPU_STK_SIZE const OSCfg_IdleTaskStkSize = (CPU_STK_SIZE)OS_CFG_IDLE_TASK_SIZE;


