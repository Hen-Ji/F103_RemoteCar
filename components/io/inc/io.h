//输入输出引脚推荐:
//PA0-PA12, PB0, PB1, PB5-PB12

#ifndef __IO_H
#define __IO_H

#include "drivers.h"
#include "iodef.h"

class Io
{
private:
	GPIO_TypeDef* port; //端口
	uint32_t pin; //引脚
	uint16_t id; //IO口序号
    uint16_t mode; //模式

    static GPIO_TypeDef * const __ports[];
    static const uint32_t __pins[];
    static const uint32_t __periphs[];
public:
    enum Mode { //IO口模式，两种基本模式(In, Out)，多种附加模式，选取方式以能唯一确定IO口模式的选取方式为准
        Input = 0x1, //输入模式
        Output = 0x2, //输出模式

        Float = 0x4, //浮空
        Analog = 0x8, //接模拟信号
        PushUp = 0x10, //带上拉电阻
        PushDown = 0x20, //带下拉电阻
        OpenDrain = 0x40, //开漏
        PushPull = 0x80, //推挽

        Alternative = 0x100 //备选，可用于复用模式，使用此模式后，引脚的输出电平由硬件控制。输入模式不应使用此模式
    };

    enum State { //IO口电平
        Low = 0, //低电平
        High = 1 //高电平
    };

    Io();

    /**
     * @brief  初始化IO口
     * @param  id IO口序号，若IO口有分组，则低4位存引脚号，高4为存端口号
     * @param  mode IO口模式
     */
    Io(uint16_t id, uint16_t mode);

    ~Io();

    /**
     * @brief  初始化IO口
     * @param  id IO口序号，若IO口有分组，则低4位存引脚号，高4为存端口号
     * @param  mode IO口模式
     */
    void init(uint16_t id, uint16_t mode);
    

    /**
     * @brief  设置IO口电平
     * @param  val 0为低电平，其他值为高电平
     */
    inline void set(uint8_t val) {
        if(!val) LL_GPIO_ResetOutputPin(port, pin);
        else LL_GPIO_SetOutputPin(port, pin);
    }

    /**
     * @brief  设置IO口电平为高电平
     */
    inline void setHigh() {
        LL_GPIO_SetOutputPin(port, pin);
    }

    /**
     * @brief  设置IO口电平为低电平
     */
    inline void setLow() {
        LL_GPIO_ResetOutputPin(port, pin);
    }

    /**
     * @brief  翻转IO口电平
     */
    inline void toggle() {
        LL_GPIO_TogglePin(port, pin);
    }

    /**
     * @brief  获取IO口输入电平
     * @retval 0为低电平，1为高电平
     */
    inline uint8_t get() {
        return LL_GPIO_IsInputPinSet(port, pin);
    }
};

#endif