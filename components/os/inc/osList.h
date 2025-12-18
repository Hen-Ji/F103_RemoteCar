#ifndef __OS_LIST_H
#define __OS_LIST_H

#include "osConfig.h"

class OsList {
private:
    void* _val;
    OsList* _prev;
    OsList* _next;
public:

    /**
     * @brief  初始化链表
     */
    OsList();

    ~OsList();

    /**
     * @brief  初始化链表
     */
    void init(void* const val = nullptr, OsList* const prev = nullptr, OsList* const next = nullptr);

    /**
     * @brief  在此节点后面添加一个节点
     * @param  node 节点
     */
    void insert(OsList* const node);

    /**
     * @brief  在此节点所在的链表中移除此节点
     */
    void remove();
    
    /**
     * @brief  获取节点值
     * @retval 节点值
     */
    inline void* val() const {
        return _val;
    }
    
    /**
     * @brief  获取前一个节点
     * @retval 节点
     */
    inline OsList* prev() const {
        return _prev;
    }
    
    /**
     * @brief  获取后一个节点
     * @retval 节点
     */
    inline OsList* next() const {
        return _next;
    }

    
    /**
     * @brief  设置节点值
     * @retval 节点值
     */
    inline void set(void* const val) {
        _val = val;
    }
};

#endif