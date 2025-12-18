#include "osTask.h"

OsList OsTask::taskList;

extern "C" void vApplicationIdleHook( void ) {
    OsTask::__idleHook();
}

extern "C" void __osSwitchedOut() {
    OsTask::__switchedOut();
}

extern "C" void __osSwitchedIn() {
    OsTask::__switchedIn();
}

OsTask::OsTask() {
}

OsTask::OsTask(void (*taskFunction)(void* args), const char *const name, uint16_t stackSize, size_t priority, void *const args) {
    create(taskFunction, name, stackSize, priority, args);
}

OsTask::~OsTask() {
    vTaskSuspendAll();
    node.remove();
    listNode.remove();
    xTaskResumeAll();

    vTaskDelete(handle); //删除任务
}

void OsTask::create(void (*taskFunction)(void* args), const char * const name, uint16_t stackSize, size_t priority, void* const args) {
    handle = 0;
    this->name = name;

    vTaskSuspendAll(); //先挂起任务调度器，防止如果有高优先级任务被创建时，任务被这个高优先级任务抢占，导致任务指针没有存进去

    xTaskCreate((TaskFunction_t)taskFunction, name, stackSize, args, priority, &handle); //创建任务
    vTaskSetThreadLocalStoragePointer(handle, 0, this); //存任务指针
    startTick = xTaskGetTickCount(); //记录任务创建时间
    this->stackSize = stackSize;
    statStartTick = startTick;
    runtime = 0;
    usage = 0;
    usageSum = 0;
    usageAvg = 0;
    updated = true;
    node.init(this);
    listNode.init(this);
    taskList.insert(&listNode);

    xTaskResumeAll(); //任务保存完再释放任务调度器
}

const char *const OsTask::getName() const {
    return name;
}

OsTask *OsTask::current() {
    if(!xTaskGetCurrentTaskHandle()) return nullptr; //需要有任务
    return (OsTask*)pvTaskGetThreadLocalStoragePointer(xTaskGetCurrentTaskHandle(), 0); //从tcb中获取任务指针
}

void OsTask::removeCurrent() {
    vTaskSuspendAll();
    OsTask* task = current();
    task->node.remove();
    task->listNode.remove();
    xTaskResumeAll();

    vTaskDelete(task->handle); //先删除任务
    delete task; //再删除任务指针
}

void OsTask::setPriority(size_t priority) {
    vTaskPrioritySet(handle, priority);
}

uint32_t OsTask::getStackSize() {
    return stackSize;
}

uint32_t OsTask::getStartTick() {
    return startTick;
}

uint16_t OsTask::getUsage() {
    return usage;
}

uint16_t OsTask::getUsageAvg() {
    return usageAvg;
}

uint32_t OsTask::getIdleStack() {
    return uxTaskGetStackHighWaterMark(handle);
}

OsTask *OsTask::getIdleTask() {
    if(!xTaskGetIdleTaskHandle()) return nullptr; //需要有任务
    return (OsTask*)pvTaskGetThreadLocalStoragePointer(xTaskGetIdleTaskHandle(), 0);
}

OsList *OsTask::getTaskList() {
    return &taskList;
}

void OsTask::__switchedOut()
{
    if(xTaskGetCurrentTaskHandle()) { //需要有任务
        OsTask* task = (OsTask*)pvTaskGetThreadLocalStoragePointer(xTaskGetCurrentTaskHandle(), 0); //从tcb中获取任务指针
        if(task && !task->updated) {
            task->updated = true;
            uint32_t tick = xTaskGetTickCount(); //获取当前tick
            uint32_t duration = (uint64_t)tick * 1000 + OS_SYSTICK_CNT*1000/OS_SYSTICK_RELOAD - task->runtime; //获取任务运行时间

            task->usage = duration > 1000 ? 1000 : duration; //计算CPU使用率
            task->usageSum += duration;

            duration = tick - task->statStartTick; //计算CPU平均使用率
            if(duration >= OS_CALCULATE_USAGE_AVG) {
                task->usageAvg = task->usageSum / duration;
                task->usageSum = 0;
                task->statStartTick = tick;
            }
        }
    }
}

void OsTask::__switchedIn() {
    if(xTaskGetCurrentTaskHandle()) { //需要有任务
        OsTask* task = (OsTask*)pvTaskGetThreadLocalStoragePointer(xTaskGetCurrentTaskHandle(), 0); //从tcb中获取任务指针
        if(task) {
            task->updated = false;
            task->runtime = (uint64_t)xTaskGetTickCount() * 1000 + OS_SYSTICK_CNT*1000/OS_SYSTICK_RELOAD; //记录开始运行的时间
        }
    }
}

void OsTask::__idleHook() {
    static uint8_t inited = false;
    static OsTask* task = nullptr;

    if(!inited) { //若空闲任务未初始化，则初始化
        vTaskSuspendAll();

        inited = true;

        task = new OsTask(); //创建空闲任务指针
        task->handle = xTaskGetCurrentTaskHandle(); //获得空闲任务句柄
        vTaskSetThreadLocalStoragePointer(task->handle, 0, task); //存任务指针

        task->name = "IDLE"; //初始化
        task->stackSize = configMINIMAL_STACK_SIZE;
        task->startTick = 0;
        task->statStartTick = task->startTick;
        task->runtime = 0;
        task->usage = 0;
        task->usageSum = 0;
        task->usageAvg = 0;
        task->updated = true;
        task->node.init(task);

        task->listNode.init(task);
        taskList.insert(&task->listNode);

        xTaskResumeAll();
    }
}
