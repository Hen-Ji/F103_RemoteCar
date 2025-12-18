#ifndef __MATRIX_H
#define __MATRIX_H

#include "drivers.h"
#include "os.h"
#include "vector.h"
#include <initializer_list>

template <typename T, size_t R, size_t C> class Matrix { //矩阵类
public:
    T _arr[R][C];

    Matrix() {};

    /**
     * @brief  初始化矩阵
     * @param  val 初始化值
     */
    Matrix(const T val) {
        init(val);
    }

    /**
     * @brief  使用初始化列表初始化矩阵
     * @param  vals 初始化列表引用
     */
    Matrix(const std::initializer_list<std::initializer_list<T>>& vals) {
        init(vals);
    }

    ~Matrix() {}

    /**
     * @brief  初始化矩阵
     * @param  val 初始化值
     */
    void init(const T val) {
        for(size_t i = 0; i < R; i++) {
            for(size_t j = 0; j < C; j++) {
                _arr[i][j] = val;
            }
        }
    }

    /**
     * @brief  使用初始化列表初始化矩阵
     * @param  vals 初始化列表引用
     */
    void init(const std::initializer_list<std::initializer_list<T>>& vals) {
        size_t i = 0, j = 0;
        for(auto vector : vals) {
            for(auto item : vector) {
                _arr[i][j++] = item;
            }
            i++;
        }
    }

    /**
     * @brief  按索引获取某行数组
     * @param  idx 索引
     * @retval 数组指针
     */
    inline T* operator[](const size_t idx) {
        return _arr[idx];
    }

    /**
     * @brief  重载等于号
     * @param  matrix 矩阵引用
     * @retval 此矩阵的引用
     */
    inline Matrix<T, R, C>& operator=(const Matrix<T, R, C>& matrix) {
        for(size_t i = 0; i < R; i++) {
            for(size_t j = 0; j < C; j++) {
                _arr[i][j] = matrix._arr[i][j];
            }
        }
        return *this;
    }

    inline Matrix<T, R, C>& operator+=(const Matrix<T, R, C>& matrix) {
        for(size_t i = 0; i < R; i++) {
            for(size_t j = 0; j < C; j++) {
                _arr[i][j] += matrix._arr[i][j];
            }
        }
        return *this;
    }
    inline Matrix<T, R, C>& operator-=(const Matrix<T, R, C>& matrix) {
        for(size_t i = 0; i < R; i++) {
            for(size_t j = 0; j < C; j++) {
                _arr[i][j] += matrix._arr[i][j];
            }
        }
        return *this;
    }

    inline Matrix<T, R, R> operator*(const Matrix<T, C, R>& matrix) {
        Matrix<T, R, R> res;
        for(size_t i = 0; i < R; i++) {
            for(size_t j = 0; j < R; j++) {
                res[i][j] = 0;
                for(size_t k = 0; k < C; k++) {
                    res[i][j] += _arr[i][k] * matrix._arr[k][j];
                }
            }
        }
        return *this;
    }

    inline Vector<T, C> operator*(Vector<T, C> vector) {
        Vector<T, C> res;
        for(size_t i = 0; i < R; i++) {
            res[i] = 0;
            for(size_t k = 0; k < C; k++) {
                res[i] += _arr[i][k] * vector[k];
            }
        }
        return res;
    }
};

#endif