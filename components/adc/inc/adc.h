/* ADC引脚定义
               ADC1   ADC2
    channel0   PA0    PA0
    channel1   PA1    PA1
    channel2   PA2    PA2
    channel3   PA3    PA3
    channel4   PA4    PA4
    channel5   PA5    PA5
    channel6   PA6    PA6
    channel7   PA7    PA7
    channel8   PB0    PB0
    channel9   PB1    PB1
    channel10  PC0    PC0
    channel11  PC1    PC1
    channel12  PC2    PC2
    channel13  PC3    PC3
    channel14  PC4    PC4
    channel15  PC5    PC5
*/

#ifndef __ADC_H
#define __ADC_H

#include "drivers.h"
#include "io.h"

class Adc
{
private:
    ADC_TypeDef * handle;
    uint8_t id;
public:
    Adc();

    /**
     * @brief  ADC初始化
     * @param  id 使用的ADC(1-2)
     */
    Adc(uint8_t id);

    ~Adc();

    /**
     * @brief  ADC初始化
     * @param  id 使用的ADC(1-2)
     */
    void init(uint8_t id);

    /**
     * @brief  获取ADC的id
     */
    uint8_t getId();
};

class AdcChannel
{
private:
    Adc* adc;
    ADC_TypeDef * handle;
    uint8_t channelId;
    uint32_t channel;
    Io io;

    static const uint32_t __channels[];
public:
    AdcChannel();

    /**
     * @brief  ADC通道初始化
     * @param  adc 使用的ADC(1-2)
     * @param  channelId ADC通道(0-15)
     * @param  io ADC输入io引脚号
     */
    AdcChannel(Adc* adc, uint8_t channelId, uint16_t io);

    ~AdcChannel();

    /**
     * @brief  ADC通道初始化
     * @param  adc 使用的ADC(1-2)
     * @param  channelId ADC通道(0-15)
     * @param  io ADC输入io引脚号
     */
    void init(Adc* adc, uint8_t channelId, uint16_t io);

    /**
     * @brief  获取ADC的值
     * @retval ADC的值(0-4095)
     */
    uint16_t get();
};

#endif