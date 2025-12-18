#ifndef __LORA_H
#define __LORA_H

#include "drivers.h"
#include "io.h"
#include "spi.h"
#include "os.h"
#include "array.h"

struct LoraStatus {
    uint8_t mode; //当前模式
    uint8_t status; //当前状态
};

struct LoraIrqStatus {
    uint8_t txDone; //发送完成
    uint8_t rxDone; //接收完成
    uint8_t preambleDetected; //检测到前导字段
    uint8_t headerValid;
    uint8_t headerErr;
    uint8_t crcErr;
    uint8_t cadDone;
    uint8_t cadDetected;
    uint8_t timeout;
};

class Lora
{
private:
    SpiDevice device; //设备
    Io rst; //复位io
    Io busy; //忙io
    Io dio; //dio
    uint32_t freq; //射频频率
    uint8_t bandwidth; //带宽
    uint8_t spreadingFactor; //扩散因子
    uint8_t codingRate; //编码率
    uint8_t mode;
    OsMutex mutex;
    bool transmitting;
    OsTask* transmitTask;

    void (*transmitCallback)(uint8_t);
    void (*receiveCallback)(uint8_t*, uint8_t);
    
    void transmitCommands(uint8_t* commands, size_t size);
    void swapBytes(uint8_t* data, uint8_t* receiveBuffer, uint32_t size);
    void transmitBytes(uint8_t* bytes, uint16_t size);

public:
    enum Mode : uint8_t {
        Standby = 0,
        Tx = 1,
        Rx = 2
    };

    enum Bandwidth : uint8_t {
        Bandwidth125KHz = 4,
        Bandwidth250KHz = 5,
        Bandwidth500KHz = 6
    };

    enum SpreadingFactor : uint8_t {
        SpreadingFactor5 = 5,
        SpreadingFactor6 = 6,
        SpreadingFactor7 = 7,
        SpreadingFactor8 = 8,
        SpreadingFactor9 = 9,
        SpreadingFactor10 = 10,
        SpreadingFactor11 = 11,
    };

    enum CodingRate : uint8_t {
        CodingRate4_5 = 1,
        CodingRate4_6 = 2,
        CodingRate4_7 = 3,
        CodingRate4_8 = 4
    };

    Lora();

    /**
     * @brief  LoRa初始化
     * @param  bus SpiBus结构体指针
     * @param  cs CS引脚号
     * @param  rst RESET引脚号
     * @param  busy BUSY引脚号
     * @param  dio DIO引脚号
     * @param  freq 射频频率
     * @param  bandwidth 带宽
     * @param  spreadingFactor 扩散因子
     * @param  codingRate 编码率
     */
    Lora(SpiBus* bus, uint16_t cs, uint16_t rst, uint16_t busy, uint16_t dio,
         uint32_t freq = 470500000, uint8_t bandwidth = Bandwidth125KHz, uint8_t spreadingFactor = SpreadingFactor9, uint8_t codingRate = CodingRate4_5);

    ~Lora();

    /**
     * @brief  LoRa初始化
     * @param  bus SpiBus结构体指针
     * @param  cs CS引脚号
     * @param  rst RESET引脚号
     * @param  busy BUSY引脚号
     * @param  dio DIO引脚号
     * @param  freq 射频频率
     * @param  bandwidth 带宽
     * @param  spreadingFactor 扩散因子
     * @param  codingRate 编码率
     */
    void init(SpiBus* bus, uint16_t cs, uint16_t rst, uint16_t busy, uint16_t dio,
        uint32_t freq = 470500000, uint8_t bandwidth = Bandwidth125KHz, uint8_t spreadingFactor = SpreadingFactor9, uint8_t codingRate = CodingRate4_5);

    /**
     * @brief  设置射频频率（默认为470.5MHz）
     * @param  freq 射频频率（单位：Hz）
     */
    void setFreq(uint32_t freq);

    /**
     * @brief  设置带宽（默认为125KHz）
     * @param  bw 带宽（LORA_BW_125KHZ，LORA_BW_250KHZ，LORA_BW_500KHZ）
     */
    void setBandwidth(uint8_t bw);

    /**
     * @brief  设置扩散因子（默认为9）
     * @param  sf 扩散因子（LORA_SF_5到LORA_SF_11）
     */
    void setSpreadingFactor(uint8_t sf);

    /**
     * @brief  设置编码率（默认为4/5）
     * @param  cr 编码率（LORA_CR_4_5到LORA_CR_4_8）
     */
    void setCodingRate(uint8_t cr);

    /**
     * @brief  设置模式
     * @param  mode LORA_STANDBY：待机模式；LORA_TX：发送模式；LORA_RX：接收模式
     */
    void setMode(uint8_t mode);

    /**
     * @brief  设置待机模式
     */
    void setStandbyMode();

    /**
     * @brief  设置发送模式
     */
    void setTransmitMode();

    /**
     * @brief  设置接收模式
     */
    void setReceiveMode();

    /**
     * @brief  发送数据
     * @param  data 发送的数据
     * @param  size 发送数据的长度
     * @param  timeout 等待时间，0为不等待，-1为一直等待
     * @retval true为发送成功，false为发送失败
     */
    bool transmit(const uint8_t *data, size_t size, int32_t timeout = -1);

    /**
     * @brief  发送初始化列表
     * @param  data 初始化列表引用
     * @param  timeout 等待时间，0为不等待，-1为一直等待
     * @retval true为发送成功，false为发送失败
     */
    bool transmit(const std::initializer_list<uint8_t>& data, int32_t timeout = -1);

    /**
     * @brief  发送数组
     * @param  data 数组引用
     * @param  timeout 等待时间，0为不等待，-1为一直等待
     * @retval true为发送成功，false为发送失败
     */
    bool transmit(const Array<uint8_t>& data, int32_t timeout = -1);

    /**
     * @brief  发送数组
     * @param  data 数组指针
     * @param  timeout 等待时间，0为不等待，-1为一直等待
     * @retval true为发送成功，false为发送失败
     */
    bool transmit(const Array<uint8_t>* data, int32_t timeout = -1);

    /**
     * @brief  接收数据
     * @param  buffer 接收缓冲指针
     * @param  size 接收到的数据长度指针
     * @param  timeout 等待时间，0为不等待，-1为一直等待
     * @retval true为接收成功，false为接收失败
     */
    bool receive(uint8_t *buffer, size_t* size, int32_t timeout = 0);

    /**
     * @brief  接收数据
     * @param  data 数组引用
     * @param  timeout 等待时间，0为不等待，-1为一直等待
     * @retval true为接收成功，false为接收失败
     */
    bool receive(Array<uint8_t>& data, int32_t timeout = 0);

    /**
     * @brief  接收数据
     * @param  data 数组指针
     * @param  timeout 等待时间，0为不等待，-1为一直等待
     * @retval true为接收成功，false为接收失败
     */
    bool receive(Array<uint8_t>* data, int32_t timeout = 0);

    /**
     * @brief  LoRa是否在忙状态
     * @retval false为空闲，true为忙
     */
    bool isBusy();

    /**
     * @brief  LoRa获取DIO1的电平
     * @retval 0为低电平，1为高电平
     */
    uint8_t getDio();

    /**
     * @brief  LoRa是否正在传输数据
     * @retval false为空闲，true为正在传输
     */
    bool isTransmitting();

    /**
     * @brief  LoRa获取设备状态
     * @retval LoraStatus 设备状态结构体
     */
    LoraStatus getStatus();

    /**
     * @brief  LoRa获取设备中断标志
     * @retval LoraIrqStatus 设备中断标志结构体
     */
    LoraIrqStatus getIrqStatus();

    /**
     * @brief  LoRa清除设备中断标志位
     * @param mask 需要清除的标志位
     */
    void clearIrqStatus(uint16_t mask);

    /**
     * @brief  LoRa获取设备寄存器的值
     * @param addr 寄存器地址
     * @retval 寄存器值
     */
    uint8_t readRegister(uint16_t addr);
};

#endif
