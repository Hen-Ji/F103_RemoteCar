#ifndef __CSTRING_H
#define __CSTRING_H

#include "drivers.h"
#include "os.h"
#include "array.h"
#include <initializer_list>
#include "format.h"
#include <stdarg.h>

class String : public Array<uint8_t> { //字符串类
private:
public:
    String();

    /**
     * @brief  初始化字符串
     * @param  capacity 字符串容量
     */
    String(const size_t capacity);

    /**
     * @brief  使用初始化列表初始化字符串
     * @param  vals 初始化列表引用
     */
    String(const std::initializer_list<char>& vals);

    /**
     * @brief  使用初始化列表初始化字符串
     * @param  vals 初始化列表引用
     */
    String(const std::initializer_list<uint8_t>& vals);

    /**
     * @brief  使用C风格字符串初始化
     * @param  vals 初始化列表引用
     */
    String(const char* const str);

    ~String();


    /**
     * @brief  初始化数组
     * @param  size 数组容量和大小
     */
    void init(const size_t size);

    /**
     * @brief  使用C风格字符串初始化
     * @param  vals 初始化列表引用
     */
    void init(const char* const str);

    /**
     * @brief  返回C风格的字符串
     */
    char* string();

    /**
     * @brief  保存格式化字符串
     * @param  format 格式化字符串
     * @param  ... 不定参数
     * @retval 此数组的引用
     */
    String& format(const char* const format, ...);

    String& append(const String& str);
    String& append(const char* const str);
    String& append(const char c);

    inline String& operator+=(const String& str) {
        return append(str);
    }

    inline String& operator+=(const char* const str) {
        return append(str);
    }

    inline String& operator+=(const char c) {
        return append(c);
    }
};

#endif