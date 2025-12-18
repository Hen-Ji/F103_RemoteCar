#ifndef __OS_H
#define __OS_H

#include "osConfig.h"
#include "osTask.h"

void* operator new(size_t size); //重载new运算符
void* operator new[](size_t size); //重载new[]运算符
void operator delete(void* ptr); //重载delete运算符
void operator delete[](void* ptr); //重载delete[]运算符
void operator delete(void* ptr, size_t num); //重载delete运算符2

class Os
{
private:

public:

    /**
      * @brief  启动操作系统，包括设置systick重装值，永不返回
      */
    static void start();

    /**
      * @brief  获取当前tick
      */
    static uint32_t getCurrentTick();

    /**
      * @brief  获取当前毫秒数
      */
    static uint32_t getCurrentMs();

    /**
      * @brief  获取当前时间(单位: 千分之一个tick)
      */
    static uint64_t getCurrentTime();

    /**
      * @brief  获取空闲堆内存
      */
    static size_t getIdleHeap();

    /**
      * @brief  任务延时一段ticks
      */
    static void delay(uint32_t tick);

    /**
      * @brief  任务延时到某个tick
      */
    static void delayTo(uint32_t tick);

    /**
      * @brief  任务延时一段时间（单位：ms）
      */
    static void delayMs(uint32_t ms);

    /**
      * @brief  任务延时到某个时间（单位：ms）
      */
    static void delayToMs(uint32_t ms);

    /**
      * @brief  放弃执行当前任务，切换上下文
      */
    static inline void yield() {
        portYIELD();
    }
    
    /**
      * @brief  进入临界区
      */
    static inline void criticalEnter() {
        taskENTER_CRITICAL();
    }

    /**
      * @brief  退出临界区
      */
    static inline void criticalExit() {
        taskEXIT_CRITICAL();
    }

    /**
      * @brief  挂起调度器
      */
    static inline void schedulerSuspend() {
        vTaskSuspendAll();
    }

    /**
      * @brief  释放调度器
      */
    static inline void schedulerResume() {
        xTaskResumeAll();
    }
};


#endif