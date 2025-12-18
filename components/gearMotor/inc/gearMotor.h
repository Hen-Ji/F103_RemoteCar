#ifndef __GEAR_MOTOR_H
#define __GEAR_MOTOR_H

#include "drivers.h"
#include "timer.h"
#include "pwm.h"
#include "motor.h"
#include "pid.h"
#include "exti.h"
#include "os.h"

class GearMotor //带编码器的直流减速电机，使用PID控制
{
private:
    Motor motor; //电机
    Exti encoderA, encoderB; //编码器中断
    PosPid<float> speedPid, anglePid; //速度环pid,角度环pid

    uint8_t encoderAState, encoderBState; //编码器AB相的状态
    float speed, currentSpeed; //电机设定速度和当前速度
    float angle, currentAngle; //电机设定角度和当前角度
    float speedMax, speedMin; //电机最大速度和最小速度（反向最大速度）
    float outMax, outMin; //电机最大输出PWM和最小输出PWM（反向最大输出PWM）
    size_t encoderCoefficient; //编码器转一圈会触发的中断的次数
    uint64_t time; //上次更新的时间
    int encoderCnt; //编码器记录值

    float updateSpeedPid(float error); //更新速度环pid
    float updateAnglePid(float error); //更新角度环pid

    void updateEncoder(); //计算当前速度和角度

    static void __encoderATriggered(void* args); //编码器A相的中断被触发
    static void __encoderBTriggered(void* args); //编码器B相的中断被触发
public:

    GearMotor();
    ~GearMotor();

    /**
     * @brief  初始化直流减速电机
     * @param  timerp 控制电机正极的定时器
     * @param  channelpId 控制电机正极的定时器通道
     * @param  iop 控制电机正极的引脚号
     * @param  timern 控制电机负极的定时器
     * @param  channelnId 控制电机负极的定时器通道
     * @param  ion 控制电机负极的引脚号
     */
    void initMotor(Timer* timerp, uint8_t channelpId, uint16_t iop, Timer* timern, uint8_t channelnId, uint16_t ion);

    /**
     * @brief  初始化编码器
     * @param  ioA 编码器A相输出引脚
     * @param  ioB 编码器B相输出引脚
     * @param  priority 外部中断优先级
     * @param  coefficient 编码器转一圈会触发的中断的次数（如果是11对级，减速比为10，则为11*2*10*2 = 440）
     */
    void initEncoder(uint16_t ioA, uint16_t ioB, uint8_t priority, size_t coefficient = 440);

    /**
     * @brief  设置速度环PID
     * @param  p 比例系数
     * @param  i 积分系数
     * @param  d 微分系数
     */
    void setSpeedPid(float p = 0.2, float i = 0.1, float d = 0.1);

    /**
     * @brief  设置角度环PID
     * @param  p 比例系数
     * @param  i 积分系数
     * @param  d 微分系数
     */
    void setAnglePid(float p = 7, float i = 0.1, float d = 0.1);

    /**
     * @brief  设置电机最大速度和最小速度（反向最大速度），单位：转/秒，默认值为 -5 到 5
     * @param  min 最小速度
     * @param  max 最大速度
     */
    void setSpeedRange(float min, float max);

    /**
     * @brief  设置电机最大输出PWM和最小输出PWM（反向最大输出PWM），默认值为 -1 到 1
     * @param  min 最小输出PWM
     * @param  max 最大输出PWM
     */
    void setOutRange(float min, float max);

    /**
     * @brief  启动电机
     */
    void start();

    /**
     * @brief  停止电机
     */
    void stop();

    /**
     * @brief  设置电机速度。
     * @param  speed 电机速度
     */
    void setSpeed(float speed);

    /**
     * @brief  更新电机速度，推荐每40ms执行一次。此函数需要搭配setSpeed使用，setAngle无效。updateSpeed与updateAngle二选一
     */
    void updateSpeed();

    /**
     * @brief  设置电机角度
     * @param  angle 电机角度
     */
    void setAngle(float angle);

    /**
     * @brief  更新电机角度，推荐每40ms执行一次。此函数需要搭配setAngle使用，setSpeed无效，但setSpeedRange有效。updateSpeed与updateAngle二选一
     */
    void updateAngle();

    /**
     * @brief  获取电机当前速度。当使用updateAngle时无效
     */
    float getCurrentSpeed();

    /**
     * @brief  获取电机当前角度。当使用updateSpeed时无效
     */
    float getCurrentAngle();

    /**
     * @brief  重置电机角度
     */
    void clearAngle();

    /**
     * @brief  获取电机速度环PID指针
     */
    PosPid<float>* getSpeedPid();

    /**
     * @brief  获取电机角度环PID指针
     */
    PosPid<float>* getAnglePid();

};

#endif
