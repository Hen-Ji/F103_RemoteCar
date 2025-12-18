#ifndef __CQUEUE_H
#define __CQUEUE_H

#include "drivers.h"
#include "os.h"

#define QUEUE_USE_DYNAMIC false //是否允许在push时可能发生的堆内存分配操作

template <typename T> class Queue {
private:
    T* _arr;
    size_t _size;
    size_t _capacity;
    size_t _begin;
public:
    Queue();

    /**
     * @brief  初始化队列
     * @param  capacity 队列容量
     */
    Queue(const size_t capacity);

    ~Queue();


    /**
     * @brief  初始化队列
     * @param  capacity 数组容量
     */
    void init(const size_t capacity);

    /**
     * @brief  增加队列容量
     * @param  increment 增量
     */
    void increaseCapacity(const size_t increment);

    /**
     * @brief  设置队列容量（原先的数据不会保存）
     * @param  capacity 容量
     */
    void setCapacity(const size_t capacity);

    /**
     * @brief  在队列末尾添加元素（若QUEUE_USE_DYNAMIC为true，可能导致new操作）
     * @param  val 元素引用
     */
    void push(const T& val);

    /**
     * @brief  返回队列第一个元素的引用，并且队列长度减1
     * @retval 元素引用
     */
    inline T& pop();

    /**
     * @brief  返回队列第一个元素的引用
     * @retval 元素引用
     */
    inline T& front() const;

    /**
     * @brief  返回队列最后一个元素的引用
     * @retval 元素引用
     */
    inline T& back() const;

    /**
     * @brief  获取队列长度
     * @retval 队列长度
     */
    inline size_t size() const;

    /**
     * @brief  获取队列容量
     * @retval 队列容量
     */
    inline size_t capacity() const;

    /**
     * @brief  判断队列是否为空，即队列长度是否为0
     * @retval true为空，false为不空
     */
    inline bool empty() const;

    /**
     * @brief  判断队列是否已满，即队列长度是否等于队列容量
     * @retval true为满，false为不满
     */
    inline bool full() const;
};

template class Queue<uint8_t>; //显式实例化

//成员函数定义

template <typename T> inline Queue<T>::Queue() {
    _arr = nullptr;
    _size = 0;
    _capacity = 0;
    _begin = 0;
}

template <typename T> inline Queue<T>::Queue(const size_t capacity) {
    _arr = nullptr;
    init(capacity);
}

template <typename T> inline Queue<T>::~Queue() {
    delete[] _arr;
}

template <typename T> inline void Queue<T>::init(const size_t capacity) {
    _size = 0;
    _capacity = capacity;
    if(_arr) delete[] _arr;
    _arr = new T[capacity];
}

template <typename T> inline void Queue<T>::increaseCapacity(const size_t increment) {
    T* preArr = _arr;
    _arr = new T[_capacity + increment];
    if(preArr) {
        for(size_t i = 0; i < _capacity; i++) _arr[i] = preArr[(_begin + i) % _capacity];
        delete[] preArr;
    }
    _capacity += increment;
}

template <typename T> inline void Queue<T>::setCapacity(const size_t capacity) {
    if(_arr) delete[] _arr;
    _arr = new T[capacity];
    _capacity = capacity;
    _size = 0;
    _begin = 0;
}

template <typename T> inline void Queue<T>::push(const T &val) {
#if (ARRAY_USE_DYNAMIC == true)
    if(_size >= _capacity) {
        increaseCapacity(_capacity * 0.5 + 1);
    }
#endif

    _arr[(_begin + _size) % _capacity] = val;
    _size++;
}

template <typename T> inline T &Queue<T>::pop() {
    T& val = _arr[_begin];
    _begin = (_begin + 1) % _capacity;
    _size--;
    return val;
}

template <typename T> inline T &Queue<T>::front() const {
    return _arr[_begin];
}

template <typename T> inline T &Queue<T>::back() const {
    return _arr[(_begin + _size - 1) % _capacity];
}

template <typename T> inline size_t Queue<T>::size() const {
    return _size;
}

template <typename T> inline size_t Queue<T>::capacity() const {
    return _capacity;
}

template <typename T> inline bool Queue<T>::empty() const {
    return _size == 0;
}

template <typename T> inline bool Queue<T>::full() const {
    return _size == _capacity;
}

#endif