#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include "drivers.h"
#include "io.h"
#include "adc.h"

struct JoystickStatus {
    float x;
    float y;
};

class Joystick
{
private:
    AdcChannel channelx;
    AdcChannel channely;
public:
    Joystick();

    /**
     * @brief  摇杆初始化
     * @param  adcx 摇杆x方向的ADC
     * @param  channelxId 摇杆x方向的ADC通道id
     * @param  iox 摇杆x方向的输入引脚
     * @param  adcy 摇杆y方向的ADC
     * @param  channelyId 摇杆y方向的ADC通道id
     * @param  ioy 摇杆y方向的输入引脚
     */
    Joystick(Adc* adcx, uint8_t channelxId, uint16_t iox, Adc* adcy, uint8_t channelyId, uint16_t ioy);
    
    ~Joystick();

    /**
     * @brief  摇杆初始化
     * @param  adcx 摇杆x方向的ADC
     * @param  channelxId 摇杆x方向的ADC通道id
     * @param  iox 摇杆x方向的输入引脚
     * @param  adcy 摇杆y方向的ADC
     * @param  channelyId 摇杆y方向的ADC通道id
     * @param  ioy 摇杆y方向的输入引脚
     */
    void init(Adc* adcx, uint8_t channelxId, uint16_t iox, Adc* adcy, uint8_t channelyId, uint16_t ioy);


    /**
     * @brief  获取摇杆状态
     */
    JoystickStatus get();
};

#endif