#include "imu.h"

#define MPU6050_ADDRESS 0x68 // 110 1000

#define	MPU6050_SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	MPU6050_CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	MPU6050_GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)。0x10: 1000deg/s, 0x08: 500deg/s
#define	MPU6050_ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)。0x10: 8g, 0x08: 4g
#define	MPU6050_ACCEL_XOUT_H	0x3B
#define	MPU6050_ACCEL_XOUT_L	0x3C
#define	MPU6050_ACCEL_YOUT_H	0x3D
#define	MPU6050_ACCEL_YOUT_L	0x3E
#define	MPU6050_ACCEL_ZOUT_H	0x3F
#define	MPU6050_ACCEL_ZOUT_L	0x40
#define	MPU6050_TEMP_OUT_H		0x41
#define	MPU6050_TEMP_OUT_L		0x42

#define	MPU6050_GYRO_XOUT_H		0x43
#define	MPU6050_GYRO_XOUT_L		0x44	
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48

#define	MPU6050_PWR_MGMT_1		0x6B	//电源管理1
#define	MPU6050_PWR_MGMT_2		0x6C	//电源管理2
#define	MPU6050_WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)

void ImuDevice::transmitCommand(uint8_t address, uint8_t command) {
    device.start(IicBus::Write);
    device.transmit({address, command});
    device.stop();
}

uint8_t ImuDevice::receiveData(uint8_t address) {
    uint8_t res = 0;
    device.start(IicBus::Write);
    device.transmit(&address, 1);
    device.start(IicBus::Read);
    device.receive(&res, 1);
    device.stop();
    return res;
}

ImuDevice::ImuDevice() {
}

ImuDevice::ImuDevice(IicBus *bus) {
    init(bus);
}

ImuDevice::~ImuDevice() {
}

void ImuDevice::init(IicBus *bus) {
    device.init(bus, MPU6050_ADDRESS);

    if (device.lock(-1)) { //初始化
        transmitCommand(MPU6050_PWR_MGMT_1, 0x01); //电源管理1，取消睡眠模式，使用陀螺仪时钟
        transmitCommand(MPU6050_PWR_MGMT_2, 0x00); //电源管理2，默认0x00
        transmitCommand(MPU6050_SMPLRT_DIV, 0x09); //10分频
        transmitCommand(MPU6050_CONFIG, 0x06); //设置低通滤波器
        transmitCommand(MPU6050_GYRO_CONFIG, 0x10); //配置陀螺仪，不自测，最大1000度/s
        transmitCommand(MPU6050_ACCEL_CONFIG, 0x10); //配置加速度计，不自测，最大8g

        device.unlock();
    }
}

ImuVector ImuDevice::getAcceleration() {
    ImuVector v(0);
	uint8_t h, l;

    if (device.lock(-1)) { //读取加速度
        h = receiveData(MPU6050_ACCEL_XOUT_H);
        l = receiveData(MPU6050_ACCEL_XOUT_L);
        v.x = (float)(int16_t)((h << 8) | l) / 4096;
        
        h = receiveData(MPU6050_ACCEL_YOUT_H);
        l = receiveData(MPU6050_ACCEL_YOUT_L);
        v.y = (float)(int16_t)((h << 8) | l) / 4096;
        
        h = receiveData(MPU6050_ACCEL_ZOUT_H);
        l = receiveData(MPU6050_ACCEL_ZOUT_L);
        v.z = (float)(int16_t)((h << 8) | l) / 4096;

        device.unlock();
    }
    return v;
}

ImuVector ImuDevice::getAngularVelocity() {
    ImuVector v(0);
	uint8_t h, l;

    if (device.lock(-1)) { //读取角速度
        h = receiveData(MPU6050_GYRO_XOUT_H);
        l = receiveData(MPU6050_GYRO_XOUT_L);
        v.x = (float)(int16_t)((h << 8) | l) / 32.768;
        
        h = receiveData(MPU6050_GYRO_YOUT_H);
        l = receiveData(MPU6050_GYRO_YOUT_L);
        v.y = (float)(int16_t)((h << 8) | l) / 32.768;
        
        h = receiveData(MPU6050_GYRO_ZOUT_H);
        l = receiveData(MPU6050_GYRO_ZOUT_L);
        v.z = (float)(int16_t)((h << 8) | l) / 32.768;

        device.unlock();
    }
    return v;
}

Imu::Imu() {
}

Imu::Imu(IicBus *bus) {
    init(bus);
}

Imu::~Imu() {
}

void Imu::init(IicBus *bus) {
    device.init(bus);
    angularVelocityBias.init(0);
    moving = false;
    motionlessCnt = 0;
    for(size_t i = 0; i < 5; i++) {
        preAv[i].init(0);
        preAc[i].init(0);
    }
    preIdx = 0;
    quaternions.init(0);
    quaternions.w = 1; //初始化四元数为(1, 0, 0, 0) ，表示没有旋转
    accelerationBias.init(0);
    accelerationFactor.init(0);
    initialGravity.init(0);
    gravity.init(0);
    time = 0;
}

ImuVector Imu::getAcceleration() {
    return device.getAcceleration();
}

ImuVector Imu::getAngularVelocity() {
    return device.getAngularVelocity();
}

void Imu::initBias(size_t count, size_t cycle) {
    Os::delayMs(100); //等待100ms，使imu稳定，否则可能得不到预期的偏置

    angularVelocityBias.init(0); //初始化
    initialGravity.init(0);

    for(size_t i = 0; i < count; i++) { //累加多次
        angularVelocityBias += getAngularVelocity();
        initialGravity += getAcceleration();
        Os::delay(cycle);
    }
    
    angularVelocityBias /= count; //取平均值
    initialGravity /= count;
    for(size_t i = 0; i < 5; i++) preAv[i] = angularVelocityBias; //保存初始值
    for(size_t i = 0; i < 5; i++) preAc[i] = initialGravity;

    accelerationBias.x = 0.089; //手动设置加速度计的偏置和缩放因子
    accelerationBias.y = -0.0112;
    accelerationBias.z = 0.0612;
    accelerationFactor.x = 1;
    accelerationFactor.y = 1;
    accelerationFactor.z = 0.97276;

    angularVelocityFactor.x = 1.00261; //手动设置陀螺仪的缩放因子
    angularVelocityFactor.y = 1.00261;
    angularVelocityFactor.z = 1.00261;

    initialGravity -= accelerationBias; //初始重力加速度减去偏置，再乘上缩放因子，并单位化，尽量靠近真实值
    initialGravity.x *= accelerationFactor.x;
    initialGravity.y *= accelerationFactor.y;
    initialGravity.z *= accelerationFactor.z;
    float norm = sqrt(initialGravity.x*initialGravity.x + initialGravity.y*initialGravity.y + initialGravity.z*initialGravity.z);
	initialGravity /= norm; //单位化
	initialGravity *= -1; //重力方向与测量值相反，加个负号
    accelerationFactor /= norm; //根据结果调整一次缩放因子

    gravity = initialGravity;
}

void Imu::update() {
    ImuVector& angularVelocity = preAv[preIdx]; //当前角速度存放位置
    ImuVector& preAngularVelocity = preAv[(preIdx + 4) % 5]; //上一次角速度引用
    ImuVector& acceleration = preAc[preIdx]; //当前加速度存放位置
    ImuVector& preAcceleration = preAc[(preIdx + 4) % 5]; //上一次加速度引用
    preIdx = (preIdx + 1) % 5;
    angularVelocity = getAngularVelocity(); //获取角速度
    acceleration = getAcceleration(); //获取加速度
    
    ImuVector v = angularVelocity - preAngularVelocity; //计算差值
    ImuVector a = acceleration - preAcceleration;
    if(v.x*v.x + v.y*v.y + v.z*v.z < 0.021 && a.x*a.x + a.y*a.y + a.z*a.z < 0.000027) { //比较稳定
        if(motionlessCnt < 49) motionlessCnt++;
        else moving = false; //连续50次稳定则表示进入静止状态
    }
    else { //不太稳定
        moving = true; //在运动中
        motionlessCnt = 0;
    }

    v = angularVelocity - angularVelocityBias; //角速度减去偏置，消除零偏
    v.x *= angularVelocityFactor.x; //角速度再乘上缩放因数，靠近真实值
    v.y *= angularVelocityFactor.y;
    v.z *= angularVelocityFactor.z;
    a = acceleration - accelerationBias; //加速度减去偏置，减少零偏
    a.x *= accelerationFactor.x; //加速度再乘上缩放因数，靠近真实值
    a.y *= accelerationFactor.y;
    a.z *= accelerationFactor.z;

    if(!moving) { //静止状态
        angularVelocityBias += v * 0.002; //使角速度偏置趋近当前角速度
        accelerationFactor += (accelerationFactor / sqrt(a.x*a.x + a.y*a.y + a.z*a.z) - accelerationFactor) * 0.002; //调整加速度缩放因数，使a的长度趋近于1
    }

    v *= IMU_PI / 180; //转为弧度制
    a *= -1; //测量的加速度与实际加速度相反，加个负号

    uint64_t t = Os::getCurrentTime(); //获取当前时间，得到上次更新到这次更新的间隔时间
    calculate(v, a, (float)(t - time) * OS_MS_PER_TICK / 1000000); //计算
    time = t; //记录当前时间
}

void Imu::calculate(ImuVector angularVelocity, ImuVector accelaration, float deltaTime) { //以imu自己为参考系，计算四元数
    ImuQuaternions& q = quaternions;
    ImuVector& v = angularVelocity;
    ImuVector& a = accelaration;
    ImuVector d;

    d.x = a.y*gravity.z - a.z*gravity.y; //a叉乘g，得到一个垂直于a和g的向量，此向量的模为|a||g|sin(alpha)=sin(alpha)约等于alpha，可用于补偿角速度
    d.y = a.z*gravity.x - a.x*gravity.z;
    d.z = a.x*gravity.y - a.y*gravity.x;
    v += d * (moving ? 0.01 : 10); //校正，如果是静止状态，则p设置大一点

    q.w = q.w + (-q.x*v.x - q.y*v.y - q.z*v.z) * deltaTime/2; //使用角速度计算四元数
	q.x = q.x + ( q.w*v.x + q.y*v.z - q.z*v.y) * deltaTime/2;
	q.y = q.y + ( q.w*v.y - q.x*v.z + q.z*v.x) * deltaTime/2;
	q.z = q.z + ( q.w*v.z + q.x*v.y - q.y*v.x) * deltaTime/2;  
	q /= sqrt(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z); //单位化

    ImuQuaternions qua = q;
    qua.w *= -1; //加个负号表示反方向旋转
    gravity = toRotationMatrix(qua) * initialGravity; //估计重力加速度向量
}

ImuVector Imu::getAngularVelocityBias() {
    return angularVelocityBias;
}

ImuVector Imu::getEulerAngle() {
    ImuVector res(0);
    ImuQuaternions& q = quaternions;
    res.x = asin(-2 * q.x * q.z + 2 * q.w* q.y); // pitch
	res.y = atan2(2 * q.y * q.z + 2 * q.w * q.x, -2 * q.x * q.x - 2 * q.y* q.y + 1); // roll
	res.z = atan2(2*(q.x*q.y + q.w*q.z),q.w*q.w+q.x*q.x-q.y*q.y-q.z*q.z); //yaw
    res *= 180 / IMU_PI; //转为角度制
    return res;
}

ImuVector Imu::getGravity() {
    return gravity;
}

ImuQuaternions Imu::getQuaternions() {
    return quaternions;
}

ImuMatrix Imu::toRotationMatrix(ImuQuaternions q) {
    ImuMatrix m;
    float &x = q.x, &y = q.y, &z = q.z, &w = q.w;
    m[0][0] = 1-2*y*y-2*z*z;
    m[0][1] = 2*x*y-2*w*z;
    m[0][2] = 2*x*z+2*w*y;
    m[1][0] = 2*x*y+2*w*z;
    m[1][1] = 1-2*x*x-2*z*z;
    m[1][2] = 2*y*z-2*w*x;
    m[2][0] = 2*x*z-2*w*y;
    m[2][1] = 2*y*z+2*w*z;
    m[2][2] = 1-2*x*x-2*y*y;
    return m;
}

bool Imu::isMoving() {
    return moving;
}
