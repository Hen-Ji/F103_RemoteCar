#ifndef __OS_GLOBAL_H
#define __OS_GLOBAL_H

#include "drivers.h"
#include "FreeRTOSConfig.h"

#include "FreeRTOS.h"
#include "task.h"

#define OS_SYSTICK_RELOAD (configCPU_CLOCK_HZ / configTICK_RATE_HZ) //设置systick重装值
#define OS_MS_PER_TICK (1000 / configTICK_RATE_HZ) //计算每运行一tick需要的时间（单位：ms）
#define OS_SYSTICK_CNT (OS_SYSTICK_RELOAD-SysTick->VAL) //节拍定时器计数值（向上计数）
#define OS_CALCULATE_USAGE_AVG (1000 / OS_MS_PER_TICK) //每过一段tick计算一次CPU平均使用率

#endif