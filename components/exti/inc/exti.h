#ifndef __EXTI_H
#define __EXTI_H

#include "drivers.h"
#include "io.h"

class Exti
{
private:
    Io io;
    uint32_t line;
    uint8_t trigger;
    IRQn_Type channelIrq;

    static const IRQn_Type __channels[];
    static const uint32_t __afLines[];

public:
    static Exti* __extis[];

    void (*__triggeredCallback)(void* args);
    void* __args;

    enum Trigger : uint8_t {
        None = 0, //不触发
        Rising = 1, //上升沿触发
        Falling = 2, //下降沿触发
        RisingFalling = 3 //上升沿和下降沿都触发
    };

    Exti();

    /**
     * @brief  外部中断初始化
     * @param  pin io口引脚号
     * @param  mode io口模式
     * @param  trigger 触发条件
     * @param  priority 中断优先级
     * @param  cb 触发回调函数
     * @param  args 函数参数
     */
    Exti(uint16_t id, uint16_t mode, uint8_t trigger, uint8_t priority = 0, void (*cb)(void*) = nullptr, void* args = nullptr);

    ~Exti();

    /**
     * @brief  外部中断初始化
     * @param  pin io口引脚号
     * @param  mode io口模式
     * @param  trigger 触发条件
     * @param  priority 中断优先级
     * @param  cb 触发回调函数
     * @param  args 函数参数
     */
    void init(uint16_t id, uint16_t mode, uint8_t trigger, uint8_t priority = 0, void (*cb)(void*) = nullptr, void* args = nullptr);

    /**
     * @brief  设置触发回调
     * @param  cb 回调函数
     * @param  args 函数参数
     */
    void setTriggeredCallback(void (*cb)(void*), void* args = nullptr);

    /**
     * @brief  设置中断优先级
     * @param  priority 优先级
     */
    void setPriority(uint8_t priority);

    /**
     * @brief  启动外部中断
     */
    void start();

    /**
     * @brief  设置触发回调并启动外部中断
     */
    void start(void (*cb)(void*), void* args = nullptr);

    /**
     * @brief  停止外部中断
     */
    void stop();

    /**
     * @brief  获取IO口指针
     */
    Io* getIo();
};


#endif
