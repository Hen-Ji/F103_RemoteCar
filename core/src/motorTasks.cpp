#include "motorTasks.h"
#include "os.h"
#include "osGroup.h"
#include "joystick.h"
#include "gearMotor.h"
#include "pid.h"
#include "exti.h"
#include "cstring.h"
#include "vector.h"

void motorTask(void *args) {
    Timer* motor1Timer = new Timer(2, 36000 - 1, 0); //周期0.5ms，即频率2kHz
    Timer* motor2Timer = new Timer(4, 36000 - 1, 0);
    GearMotor* motor1 = new GearMotor(); //直流减速电机
    GearMotor* motor2 = new GearMotor();
    motor1->initMotor(motor1Timer, 3, PA2, motor1Timer, 4, PA3); //设置电机PWM
    motor2->initMotor(motor2Timer, 2, PB7, motor2Timer, 1, PB6);
    motor1->initEncoder(PB13, PB12, 1, 440); //设置编码器
    motor2->initEncoder(PB15, PB14, 1, 440);
    motor1->setSpeedRange(-3, 3); //设置速度范围
    motor2->setSpeedRange(-3, 3);
    motor1->setOutRange(-0.8, 0.8); //设置输出PWM范围
    motor2->setOutRange(-0.8, 0.8);
    motor1Timer->enableOutput(); //开启PWM输出
    motor2Timer->enableOutput();
    motor1Timer->start(); //开启定时器
    motor2Timer->start();
    motor1->start(); //开启电机
    motor2->start();
    Vector<float, 2> speeds;

    while(1) {
        speeds = motorSpeeds.get(); //获取电机需要达到的速度
        speeds[0] *= 3;
        speeds[1] *= 3;
        motor1->setSpeed(speeds[0]);
        motor2->setSpeed(speeds[1]);
        motor1->updateSpeed(); //更新
        motor2->updateSpeed();

        speeds[0] = motor1->getCurrentSpeed(); //获取当前速度
        speeds[1] = motor2->getCurrentSpeed();
        motorCurrentSpeeds.set(speeds);

        Os::delay(40);
    }
}
