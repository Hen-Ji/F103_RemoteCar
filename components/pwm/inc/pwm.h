/*
    每个定时器和通道对应的PWM输出口：
          ch1  ch2  ch3  ch4
    tim2  PA0  PA1  PA2  PA3
    tim3  PA6  PA7  PB0  PB1
    tim4  PB6  PB7  PB8  PB9
*/

#ifndef __PWM_H
#define __PWM_H

#include "drivers.h"
#include "timer.h"
#include "io.h"

class Pwm {
private:
    Timer* timer; //定时器
    TIM_TypeDef * handle; //定时器句柄
    Io io; //io口
    uint8_t channelId; //通道Id
    uint32_t channel; //通道
    uint32_t compare; //比较值CCR

    static uint32_t __channels[];
    static TIM_TypeDef* const __tims[];
    
public:
    Pwm();

    /**
     * @brief  PWM初始化
     * @param  timer PWM使用的定时器
     * @param  io PWM输出的IO引脚号
     * @param  channelId 使用的通道(1-4)
     * @param  compare 设置比较值，比较值 / (定时器的重载值+1) = 占空比
     */
    Pwm(Timer* timer, uint16_t io, uint8_t channelId, uint32_t compare = 0);

    ~Pwm();

    /**
     * @brief  PWM初始化
     * @param  timer PWM使用的定时器
     * @param  io PWM输出的IO引脚号
     * @param  channelId 使用的通道(1-4)
     * @param  compare 设置比较值，比较值 / (定时器的重载值+1) = 占空比
     */
    void init(Timer* timer, uint16_t io, uint8_t channelId, uint32_t compare = 0);

    /**
     * @brief  启动PWM输出（定时器需要先使能输出）
     */
    void start();

    /**
     * @brief  启动PWM输出（定时器需要先使能输出）
     * @param  compare 设置比较值
     */
    void start(uint32_t compare);

    /**
     * @brief  停止PWM输出
     */
    void stop();

    /**
     * @brief  设置占空比
     * @param  duty 占空比，分辨率受重载值限制
     */
    void setDuty(float duty);

    /**
     * @brief  设置比较值
     * @param  compare 比较值
     */
    void setCompare(uint32_t compare);
};

#endif
