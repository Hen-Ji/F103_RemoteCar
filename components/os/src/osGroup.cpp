#include "osGroup.h"
#include "string.h"

OsGroup OsGroup::groupList;

OsGroupMember::OsGroupMember() {
    prev = nullptr;
    next = nullptr;
}

OsGroupMember::OsGroupMember(const char *groupName, const char* memberName) {
    join(groupName, memberName);
}

OsGroupMember::OsGroupMember(OsGroup *group, const char* memberName) {
    join(group, memberName);
}

OsGroupMember::~OsGroupMember() {
    delete (uint8_t*)message.message; //删除消息内容缓存

	Os::schedulerSuspend(); //删除群成员节点
    if(next) next->prev = prev;
    if(prev) prev->next = next;
	Os::schedulerResume();
}

bool OsGroupMember::join(const char *groupName, const char* memberName) {
    return join(OsGroup::find(groupName), memberName);
}

bool OsGroupMember::join(OsGroup *group, const char* memberName) {
    if(!group) return false; //没有这个群组

    this->name = memberName;
    this->group = group;
    lastMessageNumber = 0;
    message.sender = "";
    message.message = new uint8_t[group->messageSize]; //创建一个空间放消息内容
    prev = nullptr;
    next = nullptr;
	
	Os::schedulerSuspend(); //放入群成员链表中, 插在群主后面
    group->memberSize++;
	next = group->memberList->next;
	prev = group->memberList;
	group->memberList->next = this;
	if(next) next->prev = this;
	Os::schedulerResume();

	return true;
}

bool OsGroupMember::send(void *message, size_t size) {
    if(size > group->messageSize) return false; //不能超过最大能容纳的大小
    Os::schedulerSuspend();

    group->lastMessageNumber++;
    OsGroupMessage* msg = &group->messageList[group->lastMessageNumber % group->messageListSize]; //计算此消息应该存在哪里
    msg->sender = name; //存发送者
    msg->size = size > 0 ? size : group->messageSize; //存消息内容大小
    memcpy(msg->message, message, msg->size); //存内容

	Os::schedulerResume();
    return true;
}

OsGroupMessage *OsGroupMember::getMessage() {
    return &message;
}

bool OsGroupMember::receive(int32_t timeout)
{
    if(timeout != 0) { //可以等待
        uint32_t time = (timeout == -1 ? ~((uint32_t)0) : Os::getCurrentTick() + timeout); //计算延时，若为-1则一直等待

        while(time > Os::getCurrentTick()) { //等待新消息
            if(lastMessageNumber >= group->lastMessageNumber) Os::delay(1);
            else break;
        }
    }

    Os::schedulerSuspend();
    if(group->lastMessageNumber > lastMessageNumber + group->messageListSize) { //太久远的消息接不到了，只能接有保存的消息
		lastMessageNumber = group->lastMessageNumber - group->messageListSize;
	}

	if (lastMessageNumber >= group->lastMessageNumber) { //没有更新的消息了
        Os::schedulerResume();
		return false;
	}

    lastMessageNumber++; //接收下一条消息
    OsGroupMessage* msg = &group->messageList[group->lastMessageNumber % group->messageListSize];
	message.sender = msg->sender; //接收发送者
    message.size = msg->size; //存消息内容长度
    memcpy(message.message, msg->message, msg->size); //存内容

    Os::schedulerResume();
    return true; //返回true，接收成功
}

bool OsGroupMember::receiveLast(int32_t timeout) {
    if(timeout != 0) { //可以等待
        uint32_t time = (timeout == -1 ? ~((uint32_t)0) : Os::getCurrentTick() + timeout); //计算延时，若为-1则一直等待

        while(time > Os::getCurrentTick()) { //等待新消息
            if(lastMessageNumber >= group->lastMessageNumber) Os::delay(1);
            else break;
        }
    }

	if (lastMessageNumber >= group->lastMessageNumber) { //没有更新的消息了
		return false;
	}

    Os::schedulerSuspend();

    lastMessageNumber = group->lastMessageNumber; //接收最新一条消息
	size_t idx = lastMessageNumber % group->messageListSize;
	message.sender = group->messageList[idx].sender; //接收发送者
    memcpy(message.message, group->messageList[idx].message, group->messageSize); //存内容

    Os::schedulerResume();
    return true;
}

OsGroup::OsGroup() {
    prev = nullptr;
    next = nullptr;
}

OsGroup::OsGroup(const char *name, size_t messageSize, size_t messageListSize) {
    init(name, messageSize, messageListSize);
}

OsGroup::~OsGroup() {
	delete messageList; //删除消息列表
	delete messageBuffer; //删除消息缓存

	Os::schedulerSuspend(); //删除群组节点
    if(next) next->prev = prev;
    if(prev) prev->next = next;
	Os::schedulerResume();
}

void OsGroup::init(const char *name, size_t messageSize, size_t messageListSize) {
	this->name = name;
	this->messageListSize = messageListSize;
	this->messageSize = messageSize;
    memberList = nullptr;
	memberSize = 0;
	lastMessageNumber = 0;
    prev = nullptr;
    next = nullptr;

	messageList = new OsGroupMessage[messageListSize]; //创建消息列表
    messageBuffer = new uint8_t[messageListSize * messageSize]; //创建缓存
    for(size_t i = 0; i < messageListSize; i++) messageList[i].message = &messageBuffer[i * messageSize];

    memberList = new OsGroupMember(); //创建群主（暂时只作为头节点的功能使用）
    memberSize++;

	Os::schedulerSuspend(); //插入到群组列表
    next = groupList.next;
    prev = &groupList;
    groupList.next = this;
    if(next) next->prev = this;
	Os::schedulerResume();
}

OsGroup* OsGroup::find(const char *name) {
    Os::schedulerSuspend();
    OsGroup* node = groupList.next;
    while(node) { //只支持const char*，不支持char*
        if(node->name == name) {
	        Os::schedulerResume();
            return node;
        }
        node = node->next;
    }
	Os::schedulerResume();
    return nullptr; //没有这个群组
}
