#ifndef __OS_GROUP_H
#define __OS_GROUP_H

#include "os.h"

class OsGroup;

struct OsGroupMessage {
    const char* sender; //消息的发送者
    void* message; //消息内容指针
    size_t size; //消息内容大小
};

class OsGroupMember {
    friend class OsGroup;

private:
    const char* name; //成员名字
    OsGroup* group; //所处群组
    size_t lastMessageNumber; //最近一次接收到的消息的序号
    OsGroupMessage message; //最近一次接收到的消息存在这里
    struct OsGroupMember *prev; //上一个群成员
    struct OsGroupMember *next; //下一个群成员

public:
    OsGroupMember();

    /**
      * @brief  通过群组名称加入群组
      * @param  groupName 群组名称
      * @param  memberName 群成员名称
      */
    OsGroupMember(const char* groupName, const char* memberName);

    /**
      * @brief  通过群组指针加入群组
      * @param  group 群组指针
      * @param  memberName 群成员名称
      */
    OsGroupMember(OsGroup* group, const char* memberName);

    ~OsGroupMember();

    /**
      * @brief  通过群组名称加入群组
      * @param  groupName 群组名称
      * @param  memberName 群成员名称
      * @retval true为加入成功，false为加入失败（没有找到这个群组）
      */
    bool join(const char* groupName, const char* memberName);

    /**
      * @brief  通过群组指针加入群组
      * @param  group 群组指针
      * @param  memberName 群成员名称
      * @retval true为加入成功，false为加入失败（没有找到这个群组）
      */
    bool join(OsGroup* group, const char* memberName);

    /**
      * @brief  发送群消息
      * @param  message 消息内容指针
      * @param  size 消息内容大小，为0则按消息能容纳的最大大小来存
      * @retval true为发送成功，false为发送失败
      */
    bool send(void* message, size_t size = 0);

    /**
      * @brief  接收群消息到缓冲区中
      * @param  timeout 等待时间，0 为不等待，-1 为一直等待
      * @retval true为接收成功，false为接收失败（没有新的群消息）
      */
    bool receive(int32_t timeout = 0);

    /**
      * @brief  接收最新的群消息到缓冲区中
      * @param  timeout 等待时间，0 为不等待，-1 为一直等待
      * @retval true为接收成功，false为接收失败（没有新的群消息）
      */
    bool receiveLast(int32_t timeout = 0);

    /**
      * @brief  获取消息缓冲区指针
      * @retval 消息缓冲区指针
      */
    OsGroupMessage* getMessage();
};

class OsGroup { //群组，只建议用于任务
    friend class OsGroupMember;

private:
    const char* name; //群组名字
    size_t memberSize; //成员数
    OsGroupMember* memberList; //成员列表
    size_t lastMessageNumber; //最近一次发送的消息的序号
    size_t messageSize; //消息内容的大小
    size_t messageListSize; //群消息列表大小
    OsGroupMessage* messageList; //群消息列表
    uint8_t* messageBuffer; //群消息内容缓存
    OsGroup* prev; //上一个群组
    OsGroup* next; //下一个群组

    static OsGroup groupList; //群组列表
public:
    OsGroup();

    /**
      * @brief  创建群组
      * @param  name 群组名称
      * @param  messageSize 群消息长度
      * @param  messageListSize 群消息列表长度，即能同时存放的最大消息个数
      */
    OsGroup(const char* name, size_t messageSize, size_t messageListSize);

    ~OsGroup();

    /**
      * @brief  创建群组
      * @param  name 群组名称
      * @param  messageSize 群消息长度
      * @param  messageListSize 群消息列表长度，即能同时存放的最大消息个数
      */
    void init(const char* name, size_t messageSize, size_t messageListSize);

    /**
      * @brief  寻找群组
      * @param  name 群组名称
      * @retval 群组指针，nullptr为未找到此群组
      */
    static OsGroup* find(const char* name);
};

#endif