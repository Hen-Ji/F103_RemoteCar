#include "iicSoftware.h"

void IicBusSoftware::delay() {
    volatile uint32_t i = cycle; // "Hey compiler, I'm volatile and, you know, I can be changed by some XYZ that you're not even aware of."
    while (i--);
}

IicBusSoftware::IicBusSoftware() {
}

IicBusSoftware::IicBusSoftware(uint16_t sclId, uint16_t sdaId, uint32_t cycle) {
    init(sclId, sdaId, cycle);
}

IicBusSoftware::~IicBusSoftware() {
}

void IicBusSoftware::init(uint16_t sclId, uint16_t sdaId, uint32_t cycle) {
    mutex.init();
    this->cycle = cycle;

    scl.init(sclId, Io::Output | Io::OpenDrain); //初始化引脚，均为开漏输出
    sda.init(sdaId, Io::Output | Io::OpenDrain);

    scl.setHigh(); //初始化电平，都为高表示总线空闲
    sda.setHigh();
    if(cycle) delay();
}

bool IicBusSoftware::start(const uint8_t addr, const uint8_t direction) {
    if(cycle) { //有延时
        sda.setHigh();
        delay();
        scl.setHigh();
        delay();
        sda.setLow();
        delay();
        scl.setLow();
        delay();
    }
    else { //没有延时
        sda.setHigh(); //先释放sda, 再释放scl，保证释放时，sda为高电平
        scl.setHigh();
        sda.setLow();
        scl.setLow();
    }

    uint8_t data = (addr << 1) + direction; //合并地址和读写信号
    return transmit(&data, 1); //直接发出去
}

void IicBusSoftware::stop() {
    if(cycle) { //有延时
        sda.setLow();
        delay();
        scl.setHigh();
        delay();
        sda.setHigh();
        delay();
    }
    else { //没有延时
        sda.setLow(); //scl为高电平且sda上升为停止信号
        scl.setHigh();
        sda.setHigh();
    }
}

bool IicBusSoftware::transmit(const uint8_t *data, const size_t size) {
    bool ack = false;

    if(cycle) { //需要加延时
        for(size_t i = 0; i < size; i++) {
            for(uint8_t j = 0; j < 8; j++) {
                sda.set(data[i] & (0x80 >> j));
                delay();
                scl.setHigh();
                delay();
                scl.setLow();
                delay();
            }
            sda.setHigh();
            delay();
            scl.setHigh();
            delay();
            ack = !sda.get();
            delay();
            scl.setLow();
            delay();
        }
    }
    else { //不需要加延时
        for(size_t i = 0; i < size; i++) {
            for(uint8_t j = 0; j < 8; j++) { //高位现行
                sda.set(data[i] & (0x80 >> j));
                scl.setHigh();
                scl.setLow();
            }
        }
        sda.setHigh(); //释放sda
        scl.setHigh();
        ack = !sda.get(); //读ack
        scl.setLow();
    }

    return ack;
}

void IicBusSoftware::receive(uint8_t *buffer, const size_t size, const bool ack) {
    if(cycle) { //需要加延时
        for(size_t i = 0; i < size; i++) {
            sda.setHigh(); //释放sda
            delay();
            for(uint8_t j = 0; j < 8; j++) { //高位现行
                scl.setHigh(); //上升沿接收
                delay();
                if(sda.get() == 1) buffer[i] |= (0x80 >> j);
                delay();
                scl.setLow();
                delay();
            }
            sda.set(i == size-1 ? (ack ? 0 : 1) : 0); //发送应答
            delay();
            scl.setHigh();
            delay();
            scl.setLow();
            delay();
        }
    }
    else { //不需要加延时
        for(size_t i = 0; i < size; i++) {
            sda.setHigh(); //释放sda
            for(uint8_t j = 0; j < 8; j++) { //高位现行
                scl.setHigh(); //上升沿接收
                if(sda.get() == 1) buffer[i] |= (0x80 >> j);
                scl.setLow();
            }
            sda.set(i == size-1 ? (ack ? 0 : 1) : 0); //发送应答
            scl.setHigh();
            scl.setLow();
        }
    }
}