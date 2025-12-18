#ifndef __OS_TASK_H
#define __OS_TASK_H

#include "osConfig.h"
#include "osList.h"

typedef TaskHandle_t OsTaskHandle_t; //任务句柄

class OsTask { //任务
private:
    OsTaskHandle_t handle; //任务句柄
    const char* name; //任务名称
    uint32_t stackSize; //任务堆栈大小
    uint32_t startTick; //任务创建时间
    uint64_t runtime; //任务最近一次开始运行的时间
    uint32_t statStartTick; //统计CPU平均使用率的开始时间
    uint32_t usageSum; //任务在一段时间内CPU使用率总和
    uint16_t usage; //任务从最近一次开始运行到最近一次结束运行的时间内，CPU使用率
    uint16_t usageAvg; //任务在一段时间内CPU平均使用率
    OsList node; //用于等待事件
    OsList listNode; //任务在任务链表上的节点
    bool updated; //是否进行过一次switchedIn和switchedOut

    static OsList taskList; //任务列表
public:
    OsTask();

    /**
      * @brief  创建任务
      * @param  taskFunction 任务函数回调。任务函数示例：void xxxTask(void* args)
      * @param  name 任务名称
      * @param  stackSize 任务使用的堆栈大小
      * @param  priority 任务优先级
      * @param  args 任务函数的参数
      */
    OsTask(void (*taskFunction)(void* args), const char * const name, uint16_t stackSize, size_t priority, void* const args = nullptr);

    /**
      * @brief  删除此任务并销毁此对象
      */
    ~OsTask();

    /**
      * @brief  创建任务
      * @param  taskFunction 任务函数回调。任务函数示例：void xxxTask(void* args)
      * @param  name 任务名称
      * @param  stackSize 任务使用的堆栈大小
      * @param  priority 任务优先级
      * @param  args 任务函数的参数
      */
    void create(void (*taskFunction)(void* args), const char * const name, uint16_t stackSize, size_t priority, void* const args = nullptr);

    /**
      * @brief  重新设置此任务的任务优先级
      */
    void setPriority(size_t priority);

    /**
      * @brief  获取此任务的堆栈大小
      * @retval 堆栈大小
      */
    uint32_t getStackSize();

    /**
      * @brief  获取此任务的创建时间（单位：tick）
      * @retval 创建时间
      */
    uint32_t getStartTick();

    /**
      * @brief  获取此任务的CPU使用率
      * @retval CPU使用率（单位：0.1%）
      */
    uint16_t getUsage();

    /**
      * @brief  获取此任务的CPU平均使用率
      * @retval CPU平均使用率（单位：0.1%）
      */
    uint16_t getUsageAvg();

    /**
      * @brief  获取此任务的最小空闲栈（并不一定是实际上的最小值，可能会偏大）
      * @retval 最小空闲栈（单位：字（4 byte））
      */
    uint32_t getIdleStack();

    /**
      * @brief  获取此任务的名称
      * @retval 任务名称指针
      */
    const char* const getName() const;

    /**
      * @brief  获取当前任务
      */
    static OsTask* current();

    /**
      * @brief  删除当前任务并自动销毁此对象
      */
    static void removeCurrent();

    /**
      * @brief  获取空闲任务
      */
    static OsTask* getIdleTask();

    /**
      * @brief  获取任务列表
      */
    static OsList* getTaskList();

    /**
      * @brief  强迫停止任务阻塞状态，进入就绪状态（不建议使用，因为使用不当可能会打乱任务正常运行，除非明确知道任务在等待什么事件）
      * @retval true为任务已经进入就绪态，false为任务不在阻塞态
      */
    static inline bool abortBlocking(OsTask* task) {
        return xTaskAbortDelay(task->handle) == pdPASS;
    }

    inline OsList* __getNode() { //获得任务节点
      return &node;
    }

    static void __switchedOut(); //切换任务前计算任务状态
    static void __switchedIn(); //切换任务完成后计算任务状态
    static void __idleHook(); //空闲任务初始化
};

#endif