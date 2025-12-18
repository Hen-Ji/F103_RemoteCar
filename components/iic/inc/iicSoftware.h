#ifndef __IIC_SOFTWARE_H
#define __IIC_SOFTWARE_H

#include "iic.h"

class IicBusSoftware : public IicBus //软件IIC，适合无法使用硬件IIC的情况
{
protected:
    volatile uint32_t cycle; //延时

    void delay(); //等待一段时间

public:
    IicBusSoftware();

    /**
      * @brief  初始化软件IIC
      * @param  sclId scl引脚号
      * @param  sdaId sda引脚号
      * @param  cycle 每改变一次引脚电平后的延时时间，建议不小于1
      */
    IicBusSoftware(uint16_t sclId, uint16_t sdaId, uint32_t cycle = 1);

    ~IicBusSoftware();

    /**
      * @brief  初始化软件IIC
      * @param  sclId scl引脚号
      * @param  sdaId sda引脚号
      * @param  cycle 每改变一次引脚电平后的延时时间，建议不小于1
      */
    void init(uint16_t sclId, uint16_t sdaId, uint32_t cycle = 1);

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
};


#endif