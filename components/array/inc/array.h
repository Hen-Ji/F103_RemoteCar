#ifndef __ARRAY_H
#define __ARRAY_H

#include "drivers.h"
#include "os.h"
#include <initializer_list>

#define ARRAY_USE_DYNAMIC false //是否允许在push时可能发生的堆内存分配操作

template <typename T> class Array { //数组类，兼顾实现栈的功能
protected:
    T* _arr;
    size_t _size;
    size_t _capacity;
public:
    Array();

    /**
     * @brief  初始化数组
     * @param  size 数组容量和大小
     */
    Array(const size_t size);

    /**
     * @brief  初始化数组并初始化
     * @param  size 数组容量和大小
     * @param  val 初始值
     */
    Array(const size_t size, const T val);

    /**
     * @brief  使用初始化列表初始化数组
     * @param  vals 初始化列表引用
     */
    Array(const std::initializer_list<T>& vals);

    ~Array();


    /**
     * @brief  初始化数组
     * @param  size 数组容量和大小
     */
    void init(const size_t size);

    /**
     * @brief  增加数组容量
     * @param  increment 增量
     */
    void increaseCapacity(const size_t increment);

    /**
     * @brief  在数组末尾添加元素（若ARRAY_USE_DYNAMIC为true，可能导致new操作）
     * @param  val 元素引用
     */
    void push(const T& val);

    /**
     * @brief  在数组长度加1（若ARRAY_USE_DYNAMIC为true，可能导致new操作）
     * @param  val 元素引用
     */
    void push();

    /**
     * @brief  返回数组最后一个元素的引用，并且数组长度减1
     * @retval 元素引用
     */
    inline T& pop();

    /**
     * @brief  返回数组最后一个元素的引用
     * @retval 元素引用
     */
    inline T& top() const;

    /**
     * @brief  获取数组长度
     * @retval 数组长度
     */
    inline size_t size() const;

    /**
     * @brief  获取数组容量
     * @retval 数组容量
     */
    inline size_t capacity() const;

    /**
     * @brief  获取C语言风格的数组
     * @retval C语言风格的数组
     */
    inline T* array() const;

    /**
     * @brief  判断数组是否为空，即数组长度是否为0
     * @retval true为空，false为不空
     */
    inline bool empty() const;

    /**
     * @brief  判断数组是否已满，即数组长度是否等于数组容量
     * @retval true为满，false为不满
     */
    inline bool full() const;


    /**
     * @brief  改变数组长度（若ARRAY_USE_DYNAMIC为true，可能导致new操作）
     * @param  size 新的数组长度
     */
    void resize(const size_t size);

    /**
     * @brief  按索引获取元素引用
     * @param  idx 索引
     * @retval 元素引用
     */
    inline T& operator[](const size_t idx) const;

    /**
     * @brief  重载等于号，深拷贝赋值（将_arr的内容也复制过来）
     * @param  arr 数组引用
     * @retval 此数组的引用
     */
    Array<T>& operator=(const Array<T>& arr);
};

template class Array<uint8_t>; //显式实例化

//成员函数定义

template <typename T> Array<T>::Array() {
    _arr = nullptr;
    _size = 0;
    _capacity = 0;
}

template <typename T> Array<T>::Array(const size_t size) {
    _arr = nullptr;
    init(size);
}

template <typename T> Array<T>::Array(const size_t size, T val) {
    _arr = nullptr;
    init(size);
    for(size_t i = 0; i < size; i++) _arr[i] = val;
}

template <typename T> Array<T>::Array(const std::initializer_list<T> &vals) {
    _arr = nullptr;
    init(vals.size());
    _size = 0;
    for(auto item : vals) {
        _arr[_size++] = item;
    }
}

template <typename T> Array<T>::~Array() {
    if(_arr) delete[] _arr;
    _arr = nullptr;
}

template <typename T> void Array<T>::init(const size_t size) {
    _size = size;
    _capacity = size;
    if(_arr) delete[] _arr;
    _arr = new T[_capacity];
}

template <typename T> void Array<T>::push(const T &val) {
#if (ARRAY_USE_DYNAMIC == true)
    if(_size >= _capacity) {
        increaseCapacity(_capacity * 0.5 + 1);
    }
#endif

    _arr[_size++] = val;
}

template <typename T> void Array<T>::push() {
#if (ARRAY_USE_DYNAMIC == true)
    if(_size >= _capacity) {
        increaseCapacity(_capacity * 0.5 + 1);
    }
#endif

    _size++;
}

template <typename T> inline T& Array<T>::pop() {
    _size--;
    return _arr[_size];
}

template <typename T> inline T& Array<T>::top() const {
    return _arr[_size - 1];
}

template <typename T> void Array<T>::increaseCapacity(const size_t increment) {
    T* preArr = _arr;
    _arr = new T[_capacity + increment];
    if(preArr) {
        for(size_t i = 0; i < _capacity; i++) _arr[i] = preArr[i];
        delete[] preArr;
    }
    _capacity += increment;
}

template <typename T> inline size_t Array<T>::size() const {
    return _size;
}

template <typename T> inline size_t Array<T>::capacity() const {
    return _capacity;
}

template <typename T> inline T *Array<T>::array() const {
    return _arr;
}

template <typename T> inline bool Array<T>::empty() const {
    return _size == 0;
}

template <typename T> inline bool Array<T>::full() const {
    return _size == _capacity;
}

template <typename T> void Array<T>::resize(const size_t size) {
#if (ARRAY_USE_DYNAMIC == true)
    if(size >= _capacity) {
        increaseCapacity(size - _capacity);
    }
#endif

    _size = size;
}

template <typename T> inline T& Array<T>::operator[](const size_t idx) const {
    return _arr[idx];
}

template <typename T> Array<T> &Array<T>::operator=(const Array<T> &arr) {
#if (ARRAY_USE_DYNAMIC == true)
    if(_capacity >= arr._capacity) {
        increaseCapacity(arr._capacity - _capacity);
    }
#endif

    for(size_t i = 0; i < arr._size; i++) {
        this->_arr[i] = arr._arr[i];
    }
    this->_size = arr._size;
    return *this;
}

#endif