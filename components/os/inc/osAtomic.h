#ifndef __OS_ATOMIC_H
#define __OS_ATOMIC_H

#include "os.h"

template <typename T> class OsAtomic { //原子量，可用于任务和中断，不建议用于耗时操作
private:
    T val;
    size_t modifiedCnt;

public:
    OsAtomic() : val() {
        modifiedCnt = 0;
    }
    OsAtomic(const T& v) {
        Os::criticalEnter();
        val = v;
        modifiedCnt = 0;
        Os::criticalExit();
    }
    ~OsAtomic() {}

    void set(const T& v) { //修改
        Os::criticalEnter();
        val = v;
        modifiedCnt++; //记录修改操作
        Os::criticalExit();
    }

    T get() { //获取
        Os::criticalEnter();
        T v = val;
        Os::criticalExit();
        return v;
    }

    void operator=(const T& v) {
        set(v);
    }

    size_t getModifiedCount() { //获取修改次数
        return modifiedCnt;
    }
};

#endif