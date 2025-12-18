#ifndef __SPI_HARDWARE_H
#define __SPI_HARDWARE_H

#include "spi.h"

class SpiBusHardware : public SpiBus
{
protected:
    uint32_t freq; //频率

    SPI_TypeDef* handle; //SPI的handle
    uint8_t id;
public:
    SpiBusHardware();

    /**
      * @brief  初始化软件SPI
      * @param  miso MISO引脚号
      * @param  mosi MOSI引脚号
      * @param  sclk SCLK引脚号
      * @param  id 硬件SPI号
      * @param  freq Spi传输速率（单位: bits/s）
      */
    SpiBusHardware(uint16_t miso, uint16_t mosi, uint16_t sclk, uint8_t id, uint32_t freq = 36000000);

    ~SpiBusHardware();

    /**
      * @brief  初始化软件SPI
      * @param  miso MISO引脚号
      * @param  mosi MOSI引脚号
      * @param  sclk SCLK引脚号
      * @param  id 硬件SPI号
      * @param  freq Spi传输速率（单位: bits/s）
      */
    void init(uint16_t miso, uint16_t mosi, uint16_t sclk, uint8_t id, uint32_t freq = 36000000);

    /**
      * @brief  交换数据
      * @param  data 需要发送的数据指针，nullptr为只发送 fill 的值
      * @param  buffer 接收的数据存放的位置，nullptr为不接收，data与buffer的地址可以相同
      * @param  size 交换数据的大小
      * @param  fill 若data为nullptr，则发送fill的值
      */
    virtual void swap(const uint8_t* data, uint8_t* buffer, const size_t size, const uint8_t fill = 0xFF);
};

#endif