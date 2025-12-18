#include "iic.h"

IicBus* IicBus::__iics[] = {nullptr, nullptr};

IicBus::IicBus() {
}

IicBus::~IicBus() {
}

bool IicBus::lock(const int32_t timeout) {
    return mutex.lock(timeout);
}

bool IicBus::unlock() {
    return mutex.unlock();
}

bool IicBus::start(const uint8_t addr, const uint8_t direction) {
    return false;
}

void IicBus::stop() {
    return;
}

bool IicBus::transmit(const uint8_t *data, const size_t size) {
    return false;
}

void IicBus::receive(uint8_t *buffer, const size_t size, const bool ack) {
    return;
}

void IicBus::__irqCallback() {
    return;
}

IicDevice::IicDevice() {
    bus = nullptr;
}

IicDevice::IicDevice(IicBus *bus, uint8_t address) {
    init(bus, address);
}

IicDevice::~IicDevice() {
}

void IicDevice::init(IicBus *bus, uint8_t address) {
    this->bus = bus;
    this->address = address;
}

bool IicDevice::lock(const int32_t timeout) {
    return bus->lock(timeout);
}

bool IicDevice::unlock() {
    return bus->unlock();
}

bool IicDevice::start(const uint8_t direction) {
    return bus->start(address, direction);
}

void IicDevice::stop() {
    bus->stop();
}

bool IicDevice::transmit(const uint8_t *data, const size_t size) {
    return bus->transmit(data, size);
}

bool IicDevice::transmit(const Array<uint8_t> &data) {
    return bus->transmit(data.array(), data.size());
}

bool IicDevice::transmit(const Array<uint8_t> *data) {
    return bus->transmit(data->array(), data->size());
}

bool IicDevice::transmit(const std::initializer_list<uint8_t> &data) {
    return bus->transmit(data.begin(), data.size());
}

void IicDevice::receive(uint8_t *buffer, const size_t size, const bool ack) {
    bus->receive(buffer, size, ack);
}

void IicDevice::receive(Array<uint8_t> &buffer, const size_t size, const bool ack) {
    bus->receive(buffer.array(), size ? size : buffer.size(), ack);
}

void IicDevice::receive(Array<uint8_t> *buffer, const size_t size, const bool ack) {
    bus->receive(buffer->array(), size ? size : buffer->size(), ack);
}

IicBus *IicDevice::getBus() {
    return bus;
}

extern "C" void I2C1_EV_IRQHandler() {
    if(IicBus::__iics[0]) IicBus::__iics[0]->__irqCallback();
}

extern "C" void I2C2_EV_IRQHandler() {
    if(IicBus::__iics[1]) IicBus::__iics[1]->__irqCallback();
}