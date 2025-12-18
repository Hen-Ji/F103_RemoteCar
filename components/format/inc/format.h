#ifndef __FORMAT_H
#define __FORMAT_H

#include "drivers.h"

#include <stdarg.h>

class Format { //格式化字符串类
private:

    static size_t intFormat(char* buffer, int num, const char* const operate);
    static size_t floatFormat(char* buffer, double num, const char* const operate);
    static size_t uintFormat(char* buffer, const char* const operate);
    static size_t strFormat(char* buffer, char* str, const char* const operate);
public:
    static size_t format(char* str, const size_t maxSize, const char* const format, ...);
    static size_t vformat(char* str, const size_t maxSize, const char* const format, va_list args);

    static int toInt(const char* str, size_t* resLength = nullptr);
    static float toFloat(const char* str, size_t* resLength = nullptr);
};

#endif