#ifndef __IMU_H
#define __IMU_H

#include "drivers.h"
#include "io.h"
#include "iic.h"
#include "vector.h"
#include "matrix.h"
#include "os.h"
#include <cmath>

#define IMU_PI 3.1415926535

typedef Vector<float, 3> ImuVector;
typedef Matrix<float, 3, 3> ImuMatrix;
typedef Vector<float, 4> ImuQuaternions; //四元数(w + xi + yj + zk), 具有1个实部和三个虚部，在这里使用一个四维向量(w, x, y, z)表示，几何意义是绕一个旋转轴(x, y, z)的旋转，旋转角度与w有关

class ImuDevice
{
private:
    IicDevice device;

    void transmitCommand(uint8_t address, uint8_t command);
    uint8_t receiveData(uint8_t address);
public:
    ImuDevice();

    ImuDevice(IicBus* bus);

    ~ImuDevice();

    void init(IicBus* bus);

    ImuVector getAcceleration();
    ImuVector getAngularVelocity();
};

class Imu
{
private:
    ImuDevice device;
    ImuVector angularVelocityBias;
    ImuVector angularVelocityFactor;
    ImuVector accelerationBias;
    ImuVector accelerationFactor;
    bool moving;
    uint8_t motionlessCnt;
    ImuVector preAv[5], preAc[5];
    size_t preIdx;
    ImuVector initialGravity;
    ImuVector gravity;
    ImuQuaternions quaternions;
    uint64_t time;

public:
    Imu();
    
    Imu(IicBus* bus);

    ~Imu();

    void init(IicBus* bus);

    ImuVector getAcceleration();
    ImuVector getAngularVelocity();

    void initBias(size_t count, size_t cycle = 5);

    void update();
    void calculate(ImuVector angularVelocity, ImuVector accelaration, float deltaTime);

    ImuVector getAngularVelocityBias();
    ImuVector getEulerAngle();
    ImuVector getGravity();
    ImuQuaternions getQuaternions();
    static ImuMatrix toRotationMatrix(ImuQuaternions q);
    bool isMoving();
};


#endif
