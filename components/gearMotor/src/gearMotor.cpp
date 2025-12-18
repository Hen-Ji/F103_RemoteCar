#include "gearMotor.h"

GearMotor::GearMotor() {
    encoderAState = 0;
    encoderBState = 0;
    encoderCnt = 0;
    speed = 0;
    currentSpeed = 0;
    angle = 0;
    currentAngle = 0;
    encoderCoefficient = 0;
    time = 0;
    speedMin = -5;
    speedMax = 5;
    outMin = -1;
    outMax = 1;
    speedPid.init(0.3, 0.15, 0.1);
    anglePid.init(7, 0.1, 0.1);
}

GearMotor::~GearMotor() {
}

void GearMotor::initMotor(Timer *timerp, uint8_t channelpId, uint16_t iop, Timer *timern, uint8_t channelnId, uint16_t ion) {
    motor.init(timerp, channelpId, iop, timern, channelnId, ion);
}

void GearMotor::initEncoder(uint16_t ioA, uint16_t ioB, uint8_t priority, size_t coefficient) {
    encoderA.init(ioA, Io::Input | Io::Float, Exti::RisingFalling, priority, __encoderATriggered, this);
    encoderB.init(ioB, Io::Input | Io::Float, Exti::RisingFalling, priority, __encoderBTriggered, this);
    encoderCoefficient = coefficient;
}

void GearMotor::setSpeedPid(float p, float i, float d) {
    speedPid.init(p, i, d);
}

void GearMotor::setAnglePid(float p, float i, float d) {
    anglePid.init(p, i, d);
}

void GearMotor::setSpeedRange(float min, float max) {
    speedMin = min;
    speedMax = max;
}

void GearMotor::setOutRange(float min, float max) {
    outMin = min;
    outMax = max;
}

void GearMotor::start() {
    encoderAState = encoderA.getIo()->get();
    encoderBState = encoderB.getIo()->get();
    encoderCnt = 0;
    speed = 0;
    currentSpeed = 0;
    angle = 0;
    currentAngle = 0;
    time = Os::getCurrentTime();
    motor.start();
    motor.setSpeed(0);
    encoderA.start();
    encoderB.start();
}

void GearMotor::stop() {
    motor.stop();
    encoderA.stop();
    encoderB.stop();
}

void GearMotor::setSpeed(float speed) {
    this->speed = speed;
    if(this->speed > speedMax) this->speed = speedMax;
    else if(this->speed < speedMin) this->speed = speedMin;
}

void GearMotor::setAngle(float angle) {
    this->angle = angle;
}

void GearMotor::updateSpeed() {
    updateEncoder(); //计算当前速度和角度

    float out = updateSpeedPid(speed - currentSpeed); //更新速度pid

    if(out > 0.05) motor.setSpeed(0.4 + out * 0.6); //占空比太低转不起来
    else if(out < -0.05) motor.setSpeed(-0.4 + out * 0.6);
    else motor.setSpeed(0);
}

void GearMotor::updateAngle() {
    updateEncoder(); //计算当前速度和角度

    float out = updateAnglePid(angle - currentAngle); //更新角度pid
    out = updateSpeedPid(out - currentSpeed); //更新速度pid

    if(out > 0.05) motor.setSpeed(0.4 + out * 0.6); //占空比太低转不起来
    else if(out < -0.05) motor.setSpeed(-0.4 + out * 0.6);
    else motor.setSpeed(0);
}

float GearMotor::updateSpeedPid(float error) {
    if(error > -0.02 && error < 0.02) {
        error = 0; //在范围内就当没有误差了
    }
    speedPid.ei += error;//积分
    if(speedPid.ei > 20) speedPid.ei = 20; //积分限幅
    else if(speedPid.ei < -20) speedPid.ei = -20;

    speedPid.ed = error - speedPid.ep; //微分

    float out = speedPid.out(error); //pid控制

    if(out > outMax) out = outMax; //输出限幅
    else if(out < outMin) out = outMin;

    return out;
}

float GearMotor::updateAnglePid(float error) {
    if(error < 0.01 && error > -0.01) {
        error = 0; //在范围内就当没有误差了
    }

    if(error > 0.1 || error < -0.1) anglePid.ei = 0; //积分分离
    else anglePid.ei += error;
    if(anglePid.ei > 10) anglePid.ei = 10; //积分限幅
    else if(anglePid.ei < -10) anglePid.ei = -10;

    anglePid.ed = error - anglePid.ep; //微分

    float out = anglePid.out(error); //计算pid

    if(out > speedMax) out = speedMax; //输出限幅
    else if(out < speedMin) out = speedMin;

    return out;
}

void GearMotor::updateEncoder() {
    Os::criticalEnter();
    int cnt = encoderCnt; //获取编码器的值
    encoderCnt = 0; //清零
    Os::criticalExit();

    uint64_t nowTime = Os::getCurrentTime(); //计算当前电机速度，单位为转/秒
    currentSpeed = ((float)cnt / encoderCoefficient) / ((float)(nowTime - time) * OS_MS_PER_TICK / 1000000);
    time = nowTime;

    currentAngle += (float)cnt / encoderCoefficient; //获取当前角度
}

float GearMotor::getCurrentSpeed()
{
    return currentSpeed;
}

float GearMotor::getCurrentAngle()
{
    return currentAngle;
}

void GearMotor::clearAngle() {
    angle = 0;
    currentAngle = 0;
}

PosPid<float> *GearMotor::getSpeedPid() {
    return &speedPid;
}

PosPid<float> *GearMotor::getAnglePid()
{
    return &anglePid;
}

void GearMotor::__encoderATriggered(void *args) {
    GearMotor* motor = (GearMotor*)args; //获取电机
    uint8_t state = motor->encoderA.getIo()->get(); //获取A相电平
    if(state != motor->encoderAState) { //电平必须与上一次的电平不同，否则视为抖动(不清楚什么原因)
        motor->encoderAState = state;
        if(motor->encoderAState != motor->encoderBState) motor->encoderCnt++; //AB相电平不相同，则为逆时针旋转，否则为顺时针旋转
        else motor->encoderCnt--;
    }
}

void GearMotor::__encoderBTriggered(void *args) {
    GearMotor* motor = (GearMotor*)args; //获取电机
    uint8_t state = motor->encoderB.getIo()->get(); //获取B相电平
    if(state != motor->encoderBState) { //电平必须与上一次的电平不同，否则视为抖动(不清楚什么原因)
        motor->encoderBState = state;
        if(motor->encoderAState == motor->encoderBState) motor->encoderCnt++; //AB相电平相同，则为逆时针旋转，否则为顺时针旋转
        else motor->encoderCnt--;
    }
}
