#include "osMutex.h"

OsMutex::OsMutex() {
    init();
}

OsMutex::~OsMutex() {
}

void OsMutex::init() {
    mutex = xSemaphoreCreateRecursiveMutex();
}

bool OsMutex::lock(const int32_t timeout) {
    return xSemaphoreTakeRecursive(mutex, timeout == -1 ? ~0UL : timeout) == pdTRUE;
}
bool OsMutex::unlock() {
    return xSemaphoreGiveRecursive(mutex) == pdTRUE;
}

uint32_t OsMutex::getCount() const {
    return uxSemaphoreGetCount(mutex);
}

bool OsMutex::isLocked() const {
    return uxSemaphoreGetCount(mutex) > 0;
}
