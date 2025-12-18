#ifndef __OLED_H
#define __OLED_H

#include "drivers.h"
#include "iic.h"
#include "array.h"
#include <string.h>

#define OLED_ADDRESS 0x3C // 011 1100

class Oled
{
private:
    IicDevice device;
    uint8_t buffers[2][4][16]; //oledBuffers[0]为正在使用的缓冲，oledBuffers[1]为上次更新时的缓冲
    //使用双缓冲的好处：无论代码写得多拉，在刷新屏幕时仍能够只刷新需要刷新的地方，并且不会重复刷新，使得刷新速度大大提升
    //使用双缓冲的坏处：占用较大的RAM，对内存小的单片机不友好

    void sendCommand(const uint8_t data); //发送一位指令
    void sendCommand(const uint8_t *data, const uint8_t size); //发送多位指令
    void sendData(const uint8_t data); //发送一位数据
    void sendData(const uint8_t *data, const uint8_t size); //发送多位数据
    void setCursor(const uint8_t x, const uint8_t y); //设置指针位置
    void printChar(const uint8_t x, const uint8_t y, const char c); //向OLED写入一个字符

public:
    Oled();

    /**
     * @brief  初始化OLED
     * @param  bus OLED挂载在的IIC总线指针
     */
    Oled(IicBus* bus);

    ~Oled();

    /**
     * @brief  初始化OLED
     * @param  bus OLED挂载在的IIC总线指针
     */
    void init(IicBus* bus);
    
    /**
     * @brief  清空OLED
     */
    void clear();

    /**
     * @brief  将数据写入缓冲区中
     * @param  x 起始位置x坐标
     * @param  y 起始位置y坐标
     * @param  data 数据指针
     * @param  size 数据大小
     */
    void show(uint8_t x, uint8_t y, const uint8_t* data, size_t size);

    /**
     * @brief  将数组写入缓冲区中
     * @param  x 起始位置x坐标
     * @param  y 起始位置y坐标
     * @param  arr 数组引用
     */
    void show(uint8_t x, uint8_t y, const Array<uint8_t>& arr);

    /**
     * @brief  将数组写入缓冲区中
     * @param  x 起始位置x坐标
     * @param  y 起始位置y坐标
     * @param  arr 数组指针
     */
    void show(uint8_t x, uint8_t y, const Array<uint8_t>* arr);

    /**
     * @brief  将缓冲区中的数据发送到OLED上
     */
    void update();
};

#endif
