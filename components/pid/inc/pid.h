#ifndef __PID_H
#define __PID_H

#include "drivers.h"
#include <functional>

template <typename T> class PosPid //位置式pid
{
public:
    T kp; //比例系数
    T ki; //积分系数
    T kd; //微分系数
    T ei; //误差积分
    T ed; //误差微分
    T ep; //前一次误差

    /**
     * @brief  PID初始化
     * @param  kp 比例项P
     * @param  ki 积分项I
     * @param  kd 微分项D
     */
    PosPid(T kp = 0, T ki = 0, T kd = 0) {
        init(kp, ki, kd);
    }

    ~PosPid() { }

    /**
     * @brief  PID初始化
     * @param  kp 比例系数
     * @param  ki 积分系数
     * @param  kd 微分系数
     */
    void init(T kp = 0, T ki = 0, T kd = 0) {
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;
        this->ei = 0;
        this->ed = 0;
        this->ep = 0;
    }

    T out(T error) { //计算pid，计算前应先计算积分和微分
        ep = error; //记录这一次误差
        return kp * error + ki * ei + kd * ed; //计算pid，其中p项是主要调节手段，i项消除p项的稳态误差，d项起阻尼作用
    }
};

template <typename T> class IncPid //增量式pid
{
private:
    T kp, ki, kd;
    T errorPre, errorPrePre;
public:
    IncPid(T kp = 0, T ki = 0, T kd = 0) {
        init(kp, ki, kd);
    }

    ~IncPid() { }

    void init(T kp = 0, T ki = 0, T kd = 0) {
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;
        errorPrePre = 0;
        errorPre = 0;
    }

    void setKp(T kp) {
        this->kp = kp;
    }
    void setKi(T ki) {
        this->kp = kp;
    }
    void setKd(T kd) {
        this->kp = kp;
    }

    T out(T error) { //输出误差，输出pid增量
        T res = kp * (error - errorPre) + ki * error + kd * (error - 2 * errorPre + errorPrePre); //计算pid
        errorPrePre = errorPre; //记录状态
        errorPre = error;
        return res;
    }
};


#endif