/* SPI引脚定义
            SPI1    SPI2
    MISO    PA6     PB14
    MOSI    PA7     PB15
    SCLK    PA5     PB13
*/

#ifndef __SPI_H
#define __SPI_H

#include "drivers.h"
#include "os.h"
#include "io.h"
#include "array.h"
#include "osMutex.h"
#include <initializer_list>

class SpiBus //SPI基类
{
protected:
    Io miso, mosi, sclk; //MISO, MOSI, SCLK引脚
    OsMutex mutex; //总线锁

public:
    SpiBus();
    ~SpiBus();

    static SpiBus* __spis[];

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
      * @brief  片选使能设备
      * @param  cs 设备片选引脚
      */
    void start(Io* cs);

    /**
      * @brief  片选失能设备
      * @param  cs 设备片选引脚
      */
    void stop(Io* cs);

    /**
      * @brief  交换数据
      * @param  data 需要发送的数据指针，nullptr为只发送 fill 的值
      * @param  buffer 接收的数据存放的位置，nullptr为不接收，data与buffer的地址可以相同
      * @param  size 交换数据的大小
      * @param  fill 若data为nullptr，则发送fill的值
      */
    virtual void swap(const uint8_t* data, uint8_t* buffer, const size_t size, const uint8_t fill = 0xFF);

    virtual void __irqCallback(); //中断回调函数
};

class SpiDevice //SPI设备
{
private:
    SpiBus* bus; //挂载在的总线
    Io cs; //片选引脚号

public:
    SpiDevice();

    /**
     * @brief  初始化SPI设备
     * @param  bus 要挂载到的总线指针
     * @param  cs 片选引脚号
     */
    SpiDevice(SpiBus* bus, uint16_t cs);

    ~SpiDevice();

    /**
     * @brief  初始化SPI设备
     * @param  bus 要挂载到的总线指针
     * @param  cs 片选引脚号
     */
    void init(SpiBus* bus, uint16_t cs);

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
      * @brief  拉低片选引脚，产生开始信号
      */
    void start();

    /**
      * @brief  拉高片选引脚，产生停止信号
      */
    void stop();

    /**
      * @brief  发送数据
      * @param  data 数据指针
      * @param  size 数据长度
      */
    void transmit(const uint8_t* data, const size_t size);

    /**
      * @brief  发送数组
      * @param  data 数组引用
      */
    void transmit(const Array<uint8_t>& data);

    /**
      * @brief  发送数组
      * @param  data 数组指针
      */
    void transmit(const Array<uint8_t>* data);

    /**
      * @brief  发送初始化列表
      * @param  data 初始化列表
      */
    void transmit(const std::initializer_list<uint8_t>& data);

    /**
      * @brief  接收数据
      * @param  buffer 接收到的数据存放的位置
      * @param  size 要接收的数据长度
      * @param  fill 发送给设备的字符
      */
    void receive(uint8_t* buffer, const size_t size, const uint8_t fill = 0xFF);

    /**
      * @brief  接收数组
      * @param  buffer 接收数组引用
      * @param  fill 发送给设备的字符
      */
    void receive(Array<uint8_t>& buffer, const uint8_t fill = 0xFF);

    /**
      * @brief  接收数组
      * @param  buffer 接收数组指针
      * @param  fill 发送给设备的字符
      */
    void receive(Array<uint8_t>* buffer, const uint8_t fill = 0xFF);

    /**
      * @brief  交换数据
      * @param  data 需要发送的数据指针，nullptr为只发送 fill 的值
      * @param  buffer 接收的数据存放的位置，nullptr为不接收，data与buffer的地址可以相同
      * @param  size 交换数据的大小
      * @param  fill 若data为nullptr，则发送fill的值
      */
    void swap(const uint8_t* data, uint8_t* buffer, const size_t size, const uint8_t fill = 0xFF);

    /**
      * @brief  交换数据
      * @param  data 需要发送的初始化列表，发送的数据长度以初始化列表的长度为准
      * @param  buffer 接收的数据存放的位置
      */
    void swap(const std::initializer_list<uint8_t>& data, uint8_t* buffer);

    /**
      * @brief  交换数据
      * @param  data 需要发送的初始化列表，发送的数据长度以初始化列表的长度为准
      * @param  buffer 接收数组引用
      */
    void swap(const std::initializer_list<uint8_t>& data, Array<uint8_t>& buffer);

    /**
      * @brief  交换数据
      * @param  data 需要发送的初始化列表，发送的数据长度以初始化列表的长度为准
      * @param  buffer 接收数组指针
      */
    void swap(const std::initializer_list<uint8_t>& data, Array<uint8_t>* buffer);

    /**
      * @brief  交换数据
      * @param  data 发送数组引用，发送的数据长度以数组的长度为准
      * @param  buffer 接收数组引用
      */
    void swap(const Array<uint8_t>& data, Array<uint8_t>& buffer);

    /**
      * @brief  交换数据
      * @param  data 发送数组指针，发送的数据长度以数组的长度为准
      * @param  buffer 接收数组指针
      */
    void swap(const Array<uint8_t>* data, Array<uint8_t>* buffer);

    /**
      * @brief  交换数据
      * @param  buffer 交换数组引用
      */
    void swap(Array<uint8_t>& buffer);

    /**
      * @brief  交换数据
      * @param  buffer 交换数组指针
      */
    void swap(Array<uint8_t>* buffer);

    /**
     * @brief  获取设备挂载在的SPI总线
     * @retval SPI总线指针
     */
    SpiBus* getBus();
};

#endif
