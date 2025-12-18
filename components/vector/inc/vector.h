#ifndef __VECTOR_H
#define __VECTOR_H

#include "drivers.h"
#include "os.h"
#include <initializer_list>

template <typename T, size_t N> class Vector { //向量类
public:
    T _arr[N];

    Vector() {};

    /**
     * @brief  初始化向量
     * @param  val 初始化值
     */
    Vector(const T val) {
        init(val);
    }

    /**
     * @brief  使用初始化列表初始化向量
     * @param  vals 初始化列表引用
     */
    Vector(const std::initializer_list<T>& vals) {
        init(vals);
    }

    ~Vector() {}

    /**
     * @brief  初始化向量
     * @param  val 初始化值
     */
    void init(const T val) {
        for(size_t i = 0; i < N; i++) _arr[i] = val;
    }

    /**
     * @brief  使用初始化列表初始化向量
     * @param  vals 初始化列表引用
     */
    void init(const std::initializer_list<T>& vals) {
        size_t i = 0;
        for(auto item : vals) _arr[i++] = item;
    }

    /**
     * @brief  按索引获取元素引用
     * @param  idx 索引
     * @retval 元素引用
     */
    inline T& operator[](const size_t idx) {
        return _arr[idx];
    }

    /**
     * @brief  重载等于号
     * @param  vector 向量引用
     * @retval 此向量的引用
     */
    inline Vector<T, N>& operator=(const Vector<T, N>& vector) {
        for(size_t i = 0; i < N; i++) _arr[i] = vector._arr[i];
        return *this;
    }

    inline Vector<T, N>& operator+=(const Vector<T, N>& vector) {
        for(size_t i = 0; i < N; i++) _arr[i] += vector._arr[i];
        return *this;
    }
    inline Vector<T, N>& operator-=(const Vector<T, N>& vector) {
        for(size_t i = 0; i < N; i++) _arr[i] -= vector._arr[i];
        return *this;
    }
    inline Vector<T, N>& operator*=(const T& val) {
        for(size_t i = 0; i < N; i++) _arr[i] *= val;
        return *this;
    }
    inline Vector<T, N>& operator/=(const T& val) {
        for(size_t i = 0; i < N; i++) _arr[i] /= val;
        return *this;
    }

    inline Vector<T, N> operator+(const Vector<T, N>& vector) {
        Vector<T, N> res;
        for(size_t i = 0; i < N; i++) res[i] = _arr[i] + vector._arr[i];
        return res;
    }
    inline Vector<T, N> operator-(const Vector<T, N>& vector) {
        Vector<T, N> res;
        for(size_t i = 0; i < N; i++) res[i] = _arr[i] - vector._arr[i];
        return res;
    }
    inline Vector<T, N> operator*(const T& val) {
        Vector<T, N> res;
        for(size_t i = 0; i < N; i++) res[i] = _arr[i] * val;
        return res;
    }
    inline Vector<T, N> operator/(const T& val) {
        Vector<T, N> res;
        for(size_t i = 0; i < N; i++) res[i] = _arr[i] / val;
        return res;
    }
};



template <typename T> class Vector<T, 2> //经过优化的二维向量类
{
public:
    T x, y;

    Vector() {}
    Vector(const T val) {
        x = val;
        y = val;
    }
    ~Vector() {}

    void init(const T val) {
        x = val;
        y = val;
    }

    inline T& operator[](const size_t idx) {
        if(idx == 0) return x;
        return y;
    }

    inline Vector<T, 2>& operator=(const Vector<T, 2>& vector) {
        x = vector.x;
        y = vector.y;
        return *this;
    }
    inline Vector<T, 2>& operator+=(const Vector<T, 2>& vector) {
        x += vector.x;
        y += vector.y;
        return *this;
    }
    inline Vector<T, 2>& operator-=(const Vector<T, 2>& vector) {
        x -= vector.x;
        y -= vector.y;
        return *this;
    }
    inline Vector<T, 2>& operator*=(const T& val) {
        x *= val;
        y *= val;
        return *this;
    }
    inline Vector<T, 2>& operator/=(const T& val) {
        x /= val;
        y /= val;
        return *this;
    }

    inline Vector<T, 2> operator+(const Vector<T, 2>& vector) {
        Vector<T, 2> res;
        res.x = x + vector.x;
        res.y = y + vector.y;
        return res;
    }
    inline Vector<T, 2> operator-(const Vector<T, 2>& vector) {
        Vector<T, 2> res;
        res.x = x - vector.x;
        res.y = y - vector.y;
        return res;
    }
    inline Vector<T, 2> operator*(const T& val) {
        Vector<T, 2> res;
        res.x = x * val;
        res.y = y * val;
        return res;
    }
    inline Vector<T, 2> operator/(const T& val) {
        Vector<T, 2> res;
        res.x = x / val;
        res.y = y / val;
        return res;
    }
};



template <typename T> class Vector<T, 3> //经过优化的三维向量类
{
public:
    T x, y, z;

    Vector() {}
    Vector(const T val) {
        x = val;
        y = val;
        z = val;
    }
    ~Vector() {}

    void init(const T val) {
        x = val;
        y = val;
        z = val;
    }

    inline T& operator[](const size_t idx) {
        if(idx == 0) return x;
        if(idx == 1) return y;
        return z;
    }

    inline Vector<T, 3>& operator=(const Vector<T, 3>& vector) {
        x = vector.x;
        y = vector.y;
        z = vector.z;
        return *this;
    }
    inline Vector<T, 3>& operator+=(const Vector<T, 3>& vector) {
        x += vector.x;
        y += vector.y;
        z += vector.z;
        return *this;
    }
    inline Vector<T, 3>& operator-=(const Vector<T, 3>& vector) {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
        return *this;
    }
    inline Vector<T, 3>& operator*=(const T& val) {
        x *= val;
        y *= val;
        z *= val;
        return *this;
    }
    inline Vector<T, 3>& operator/=(const T& val) {
        x /= val;
        y /= val;
        z /= val;
        return *this;
    }

    inline Vector<T, 3> operator+(const Vector<T, 3>& vector) {
        Vector<T, 3> res;
        res.x = x + vector.x;
        res.y = y + vector.y;
        res.z = z + vector.z;
        return res;
    }
    inline Vector<T, 3> operator-(const Vector<T, 3>& vector) {
        Vector<T, 3> res;
        res.x = x - vector.x;
        res.y = y - vector.y;
        res.z = z - vector.z;
        return res;
    }
    inline Vector<T, 3> operator*(const T& val) {
        Vector<T, 3> res;
        res.x = x * val;
        res.y = y * val;
        res.z = z * val;
        return res;
    }
    inline Vector<T, 3> operator/(const T& val) {
        Vector<T, 3> res;
        res.x = x / val;
        res.y = y / val;
        res.z = z / val;
        return res;
    }
};

template <typename T> class Vector<T, 4> //经过优化的四维向量类
{
public:
    T x, y, z, w;

    Vector() {}
    Vector(const T val) {
        x = val;
        y = val;
        z = val;
        w = val;
    }
    ~Vector() {}

    void init(const T val) {
        x = val;
        y = val;
        z = val;
        w = val;
    }

    inline T& operator[](const size_t idx) {
        if(idx == 0) return x;
        if(idx == 1) return y;
        if(idx == 2) return z;
        return w;
    }

    inline Vector<T, 4>& operator=(const Vector<T, 4>& vector) {
        x = vector.x;
        y = vector.y;
        z = vector.z;
        w = vector.w;
        return *this;
    }
    inline Vector<T, 4>& operator+=(const Vector<T, 4>& vector) {
        x += vector.x;
        y += vector.y;
        z += vector.z;
        w += vector.w;
        return *this;
    }
    inline Vector<T, 4>& operator-=(const Vector<T, 4>& vector) {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
        w -= vector.w;
        return *this;
    }
    inline Vector<T, 4>& operator*=(const T& val) {
        x *= val;
        y *= val;
        z *= val;
        w *= val;
        return *this;
    }
    inline Vector<T, 4>& operator/=(const T& val) {
        x /= val;
        y /= val;
        z /= val;
        w /= val;
        return *this;
    }

    inline Vector<T, 4> operator+(const Vector<T, 4>& vector) {
        Vector<T, 4> res;
        res.x = x + vector.x;
        res.y = y + vector.y;
        res.z = z + vector.z;
        res.w = w + vector.w;
        return res;
    }
    inline Vector<T, 4> operator-(const Vector<T, 4>& vector) {
        Vector<T, 4> res;
        res.x = x - vector.x;
        res.y = y - vector.y;
        res.z = z - vector.z;
        res.w = w - vector.w;
        return res;
    }
    inline Vector<T, 4> operator*(const T& val) {
        Vector<T, 4> res;
        res.x = x * val;
        res.y = y * val;
        res.z = z * val;
        res.w = w * val;
        return res;
    }
    inline Vector<T, 4> operator/(const T& val) {
        Vector<T, 4> res;
        res.x = x / val;
        res.y = y / val;
        res.z = z / val;
        res.w = w / val;
        return res;
    }
};

#endif