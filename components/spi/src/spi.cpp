#include "spi.h"
#include "os.h"

SpiBus* SpiBus::__spis[] = {nullptr, nullptr};

SpiBus::SpiBus() {
}

SpiBus::~SpiBus() {
}

bool SpiBus::lock(const int32_t timeout) {
    return mutex.lock(timeout);
}

bool SpiBus::unlock() {
    return mutex.unlock();
}

void SpiBus::start(Io *cs) {
    cs->setLow(); //连接到从机的片选置低电平，表示要开始给从机交换数据
}

void SpiBus::stop(Io *cs) {
    cs->setHigh(); //片选置高电平，表示交换结束
}

void SpiBus::swap(const uint8_t *data, uint8_t *buffer, const size_t size, const uint8_t fill) {
    return;
}

void SpiBus::__irqCallback() {
    return;
}

SpiDevice::SpiDevice() {
    bus = nullptr;
}

SpiDevice::SpiDevice(SpiBus *bus, uint16_t cs) {
    init(bus, cs);
}

SpiDevice::~SpiDevice() {
}

void SpiDevice::init(SpiBus *bus, uint16_t cs) {
    this->bus = bus;
    this->cs.init(cs, Io::Output | Io::PushPull); //cs引脚使用推挽输出
    this->cs.setHigh(); //初始化为高电平
}

bool SpiDevice::lock(const int32_t timeout) {
    return bus->lock();
}

bool SpiDevice::unlock() {
    return bus->unlock();
}

void SpiDevice::start() {
    bus->start(&cs);
}

void SpiDevice::stop() {
    bus->stop(&cs);
}

void SpiDevice::transmit(const uint8_t *data, const size_t size) {
    bus->swap(data, nullptr, size);
}

void SpiDevice::transmit(const Array<uint8_t> &data) {
    bus->swap(data.array(), nullptr, data.size());
}

void SpiDevice::transmit(const Array<uint8_t> *data) {
    bus->swap(data->array(), nullptr, data->size());
}

void SpiDevice::transmit(const std::initializer_list<uint8_t> &data) {
    bus->swap(data.begin(), nullptr, data.size());
}

void SpiDevice::receive(uint8_t *buffer, const size_t size, const uint8_t fill) {
    bus->swap(nullptr, buffer, size, fill);
}

void SpiDevice::receive(Array<uint8_t> &buffer, const uint8_t fill) {
    bus->swap(nullptr, buffer.array(), buffer.size(), fill);
}

void SpiDevice::receive(Array<uint8_t> *buffer, const uint8_t fill) {
    bus->swap(nullptr, buffer->array(), buffer->size(), fill);
}

void SpiDevice::swap(const uint8_t *data, uint8_t *buffer, const size_t size, const uint8_t fill) {
    bus->swap(data, buffer, size, fill);
}

void SpiDevice::swap(const std::initializer_list<uint8_t> &data, uint8_t *buffer) {
    bus->swap(data.begin(), buffer, data.size());
}

void SpiDevice::swap(const std::initializer_list<uint8_t> &data, Array<uint8_t> &buffer) {
    bus->swap(data.begin(), buffer.array(), data.size());
}

void SpiDevice::swap(const std::initializer_list<uint8_t> &data, Array<uint8_t> *buffer) {
    bus->swap(data.begin(), buffer->array(), data.size());
}

void SpiDevice::swap(const Array<uint8_t> &data, Array<uint8_t> &buffer) {
    bus->swap(data.array(), buffer.array(), data.size());
}

void SpiDevice::swap(const Array<uint8_t> *data, Array<uint8_t> *buffer) {
    bus->swap(data->array(), buffer->array(), data->size());
}

void SpiDevice::swap(Array<uint8_t> &buffer) {
    bus->swap(buffer.array(), buffer.array(), buffer.size());
}

void SpiDevice::swap(Array<uint8_t> *buffer) {
    bus->swap(buffer->array(), buffer->array(), buffer->size());
}

SpiBus *SpiDevice::getBus() {
    return bus;
}
