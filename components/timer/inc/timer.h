#ifndef __TIMER_H
#define __TIMER_H

#include "drivers.h"

class Timer
{
private:
    TIM_TypeDef * handle;
    uint16_t reload;
    uint16_t prescaler;
    uint8_t priority;
    IRQn_Type channelIrq;
    uint8_t id; //定时器id
    bool output; //是否开启比较/捕获输出

    static TIM_TypeDef* const __tims[];
    static const uint32_t __periphs[];
    static const IRQn_Type __channelIrqs[];
    
public:
    static Timer* __timers[];

    void (*__timeoutCallback)(void* args);
    void* __args;

    Timer();

    /**
     * @brief  初始化定时器
     * @param  id 定时器id
     * @param  reload 重载值
     * @param  prescaler 预分频值
     * @param  priority 中断优先级
     * @param  cb 超时回调函数
     * @param  args 函数参数
     */
    Timer(uint8_t id, uint16_t reload, uint16_t prescaler = 0, uint8_t priority = 0, void (*cb)(void*) = nullptr, void* args = nullptr);

    ~Timer();

    /**
     * @brief  初始化定时器
     * @param  id 定时器id
     * @param  reload 重载值
     * @param  prescaler 预分频值
     * @param  priority 中断优先级
     * @param  cb 超时回调函数
     * @param  args 函数参数
     */
    void init(uint8_t id, uint16_t reload, uint16_t prescaler = 0, uint8_t priority = 0, void (*cb)(void*) = nullptr, void* args = nullptr);

    /**
     * @brief  启动定时器，若不设置超时回调函数（即超时回调函数为nullptr）则不开启超时中断
     */
    void start();

    /**
     * @brief  启动定时器，若不设置超时回调函数（即超时回调函数为nullptr）则不开启超时中断
     * @param  cb 超时回调函数
     * @param  args 函数参数
     */
    void start(void (*cb)(void*), void* args = nullptr);

    /**
     * @brief  停止定时器
     */
    void stop();

    /**
     * @brief  设置中断优先级
     * @param  priority 优先级
     */
    void setPriority(uint8_t priority);

    /**
     * @brief  设置超时回调（设置前应保证定时器不在运行状态）
     * @param  cb 回调函数
     * @param  args 函数参数
     */
    void setTimeoutCallback(void (*cb)(void*), void* args = nullptr);

    /**
     * @brief  获取重载值
     */
    uint32_t getReload();

    /**
     * @brief  获取使用的定时器
     */
    uint8_t getId();

    /**
     * @brief  使能比较/捕获输出，若使用PWM需要开启这个
     */
    void enableOutput();

    /**
     * @brief  失能比较/捕获输出
     */
    void disableOutput();
};


#endif
