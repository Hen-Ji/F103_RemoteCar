#include "osList.h"
#include "os.h"

OsList::OsList() {
    _val = nullptr;
    _prev = nullptr;
    _next = nullptr;
}

OsList::~OsList() {
}

void OsList::init(void * const val, OsList * const prev, OsList * const next) {
    _val = val;
    _prev = prev;
    _next = next;
}

void OsList::insert(OsList * const node) {
    //Os::criticalEnter();
    node->remove(); //先从原先的链表中移除（如有）

    node->_next = _next;
    node->_prev = this;
    if(_next) _next->_prev = node;
    _next = node;
    //Os::criticalExit();
}

void OsList::remove() {
    //Os::criticalEnter();
    if(_prev) _prev->_next = _next;
    if(_next) _next->_prev = _prev;
    _prev = nullptr;
    _next = nullptr;
    //Os::criticalExit();
}