#ifndef __OS_MUTEX_H
#define __OS_MUTEX_H

#include "os.h"
#include "osTask.h"
#include "osList.h"
#include "semphr.h"

class OsMutex { //递归互斥锁，只建议用于任务
private:
    SemaphoreHandle_t mutex; //递归互斥锁的句柄
public:

    /**
     * @brief  初始化递归互斥锁
     */
    OsMutex();

    ~OsMutex();

    /**
     * @brief  初始化递归互斥锁
     */
    void init();

    /**
     * @brief  递归互斥锁上锁
     * @param  timeout 等待时间，0 为不等待，-1 为一直等待
     * @retval true为上锁成功，false为上锁失败（已经被其他任务上锁）
     */
    bool lock(const int32_t timeout = 0);

    /**
     * @brief  递归互斥锁解锁，只能解锁当前任务上的锁
     * @retval true为解锁成功，false为解锁失败（并未上锁，或者不是当前任务上的锁）
     */
    bool unlock();

    /**
     * @brief  获取递归互斥锁上锁次数
     * @retval 上锁次数
     */
    uint32_t getCount() const;

    /**
     * @brief  查询递归互斥锁是否上锁
     * @retval true为已经上锁，false为没有上锁
     */
    bool isLocked() const;
};

#endif