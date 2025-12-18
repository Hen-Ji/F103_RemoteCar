#include "os.h"

void* operator new(size_t size) { //重载new运算符（不建议在中断中调用）
    return pvPortMalloc(size); //使用FreeRTOS提供的malloc
}

void *operator new[](size_t size) { //重载new[]运算符（不建议在中断中调用）
    return pvPortMalloc(size); //使用FreeRTOS提供的malloc
}

void operator delete(void* ptr) { //重载delete运算符（不建议在中断中调用）
    vPortFree(ptr); //使用FreeRTOS提供的free
}
void operator delete[](void *ptr) {
    vPortFree(ptr); //使用FreeRTOS提供的free
}

void operator delete(void *ptr, size_t num) { //重载delete运算符2（不建议在中断中调用）
    vPortFree(ptr); //使用FreeRTOS提供的free
}

void Os::start() {
    SysTick_Config(OS_SYSTICK_RELOAD); //设置systick重装值
	vTaskStartScheduler(); //启动任务调度器
	while(1); //一般情况不会运行到这里
}

uint32_t Os::getCurrentTick() {
    return (uint32_t)xTaskGetTickCount();
}

uint32_t Os::getCurrentMs() {
    return (uint32_t)(xTaskGetTickCount() * OS_MS_PER_TICK);
}

uint64_t Os::getCurrentTime() {
    return (uint64_t)xTaskGetTickCount() * 1000 + OS_SYSTICK_CNT*1000/OS_SYSTICK_RELOAD;
}

size_t Os::getIdleHeap() {
    return xPortGetFreeHeapSize();
}

void Os::delay(uint32_t tick) {
    vTaskDelay(tick);
}

void Os::delayTo(uint32_t tick) {
    uint32_t current = Os::getCurrentTick();
    if(tick > current) vTaskDelay(tick - current);
    else vTaskDelay(1); //最少要延时1tick
}
    

void Os::delayMs(uint32_t ms) {
    vTaskDelay(ms / OS_MS_PER_TICK);
}

void Os::delayToMs(uint32_t ms) {
    uint32_t current = Os::getCurrentMs();
    if(ms > current + OS_MS_PER_TICK) vTaskDelay((ms - current) / OS_MS_PER_TICK);
    else vTaskDelay(1); //最少要延时1tick
}