/* I2C引脚定义
          I2C1    I2C2
    SCL   PB6     PB10
    SDA   PB7     PB11
*/

#ifndef __IIC_H
#define __IIC_H

#include "drivers.h"
#include "io.h"
#include "os.h"
#include "osMutex.h"
#include "array.h"
#include <initializer_list>

class IicBus //IIC总线基类
{
protected:
    Io scl, sda; //scl，sda的引脚
    OsMutex mutex; //总线锁

public:
    enum Direction : uint8_t {
        Write = 0, //写入
        Read = 1 //读取
    };

    IicBus();
    ~IicBus();

    static IicBus* __iics[];

    /**
      * @brief  请求独占使用权
      * @param  timeout 等待时间，0为不等待，-1为一直等待
      * @retval true为上锁成功，false为上锁失败
      */
    bool lock(const int32_t timeout = 0);

    /**
      * @brief  请求放弃使用权
      * @retval true为解锁成功，false为解锁失败
      */
    bool unlock();

    /**
      * @brief  产生开始信号
      * @param  addr 7为从机地址
      * @param  direction 方向，IicBus::Write为写入，IicBus::Read为读取
      * @retval 应答值，false为NACK，总线上没有找到此设备；true为ACK，总线上有挂载此设备
      */
    virtual bool start(const uint8_t addr, const uint8_t direction);

    /**
      * @brief  产生停止信号
      */
    virtual void stop();

    /**
      * @brief  发送数据
      * @param  data 数据指针
      * @param  size 数据长度
      * @retval 应答值，false为NACK，true为ACK
      */
    virtual bool transmit(const uint8_t* data, const size_t size);

    /**
      * @brief  接收数据
      * @param  buffer 接收到的数据存放的位置
      * @param  size 要接收的数据长度
      * @param  ack 接收完成最后一个字节后发送的应答值，false为NACK，true为ACK
      */
    virtual void receive(uint8_t* buffer, const size_t size, const bool ack = false);

    virtual void __irqCallback(); //中断回调函数
};

class IicDevice //IIC设备
{
private:
    IicBus* bus; //挂载在的总线
    uint8_t address; //7位设备地址（存低7位上）

public:
    IicDevice();

    /**
     * @brief  初始化IIC设备
     * @param  bus 要挂载到的总线指针
     * @param  address 设备地址（存低7位上）
     */
    IicDevice(IicBus* bus, uint8_t address);

    ~IicDevice();

    /**
     * @brief  初始化IIC设备
     * @param  bus 要挂载到的总线指针
     * @param  address 设备地址（存低7位上）
     */
    void init(IicBus* bus, uint8_t address);

    /**
      * @brief  请求独占总线使用权
      * @param  timeout 等待时间，0为不等待，-1为一直等待
      * @retval true为上锁成功，false为上锁失败
      */
    bool lock(const int32_t timeout = 0);

    /**
      * @brief  请求放弃总线使用权
      * @retval true为解锁成功，false为解锁失败
      */
    bool unlock();

    /**
      * @brief  产生开始信号
      * @param  direction 方向，IicBus::Write为写入，IicBus::Read为读取
      * @retval 应答值，false为NACK，总线上没有找到此设备；true为ACK，总线上有挂载此设备
      */
    bool start(const uint8_t direction);

    /**
      * @brief  产生停止信号
      */
    void stop();

    /**
      * @brief  发送数据
      * @param  data 数据指针
      * @param  size 数据长度
      * @retval 应答值，false为NACK，true为ACK
      */
    bool transmit(const uint8_t* data, const size_t size);

    /**
      * @brief  发送数组
      * @param  data 数组引用
      * @retval 应答值，false为NACK，true为ACK
      */
    bool transmit(const Array<uint8_t>& data);

    /**
      * @brief  发送数组
      * @param  data 数组指针
      * @retval 应答值，false为NACK，true为ACK
      */
    bool transmit(const Array<uint8_t>* data);

    /**
      * @brief  发送初始化列表
      * @param  data 初始化列表
      * @retval 应答值，false为NACK，true为ACK
      */
    bool transmit(const std::initializer_list<uint8_t>& data);


    /**
      * @brief  接收数据
      * @param  buffer 接收到的数据存放的位置
      * @param  size 要接收的数据长度
      * @param  ack 接收完成最后一个字节后发送的应答值，false为NACK，true为ACK
      */
    void receive(uint8_t* buffer, const size_t size, const bool ack = false);

    /**
      * @brief  接收数组
      * @param  buffer 接收数组引用
      * @param  size 要接收的数据长度，若为0则以数组的长度为准
      * @param  ack 接收完成最后一个字节后发送的应答值，false为NACK，true为ACK
      */
    void receive(Array<uint8_t>& buffer, const size_t size = 0, const bool ack = false);

    /**
      * @brief  接收数组
      * @param  buffer 接收数组指针
      * @param  size 要接收的数据长度，若为0则以数组的长度为准
      * @param  ack 接收完成最后一个字节后发送的应答值，false为NACK，true为ACK
      */
    void receive(Array<uint8_t>* buffer, const size_t size = 0, const bool ack = false);

    /**
     * @brief  获取设备挂载在的IIC总线
     * @retval IIC总线指针
     */
    IicBus* getBus();
};

#endif
