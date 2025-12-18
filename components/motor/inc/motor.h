#ifndef __MOTOR_H
#define __MOTOR_H

#include "drivers.h"
#include "timer.h"
#include "pwm.h"

class Motor
{
private:
    Pwm pwmp;
    Pwm pwmn;
public:
    Motor();

    /**
     * @brief  直流电机初始化
     * @param  timerp 控制电机正极的定时器
     * @param  channelpId 控制电机正极的定时器通道
     * @param  iop 控制电机正极的引脚号
     * @param  timern 控制电机负极的定时器
     * @param  channelnId 控制电机负极的定时器通道
     * @param  ion 控制电机负极的引脚号
     */
    Motor(Timer* timerp, uint8_t channelpId, uint16_t iop, Timer* timern, uint8_t channelnId, uint16_t ion);

    ~Motor();

    /**
     * @brief  直流电机初始化
     * @param  timerp 控制电机正极的定时器
     * @param  channelpId 控制电机正极的定时器通道
     * @param  iop 控制电机正极的引脚号
     * @param  timern 控制电机负极的定时器
     * @param  channelnId 控制电机负极的定时器通道
     * @param  ion 控制电机负极的引脚号
     */
    void init(Timer* timerp, uint8_t channelpId, uint16_t iop, Timer* timern, uint8_t channelnId, uint16_t ion);


    /**
     * @brief  直流电机启动
     */
    void start();

    /**
     * @brief  直流电机停止
     */
    void stop();

    /**
     * @brief  直流电机设置速度（-1 到 1）
     */
    void setSpeed(float val);
};

#endif
