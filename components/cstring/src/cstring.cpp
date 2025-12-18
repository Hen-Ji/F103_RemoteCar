#include "cstring.h"

// 模板类的定义只能放在.h文件中，否则就要显式实例化
String::String() : Array<uint8_t>() {
}

String::String(const size_t capacity) : Array<uint8_t>(capacity) {
    _size = 0;
}

String::String(const std::initializer_list<char> &vals) : Array<uint8_t>((std::initializer_list<uint8_t>&)vals) {
}

String::String(const std::initializer_list<uint8_t> &vals) : Array<uint8_t>(vals) {
}

String::String(const char *const str) {
    _arr = nullptr;
    init(str);
}

String::~String() {
    if(_arr) delete[] _arr;
    _arr = nullptr;
}

void String::init(const size_t size) {
    Array<uint8_t>::init(size);
}

void String::init(const char *const str) {
    _size = 0;
    while(str[_size] != '\0') _size++; //计算字符串长度
    
    if(_arr) delete[] _arr;
    _arr = new uint8_t[_size]; //new一个空间存数据
    _capacity = _size;
}

char *String::string() {
    if(_size < _capacity) _arr[_size] = 0; //容量足够就直接在后面加结束符
    else { //否则就使用push
        push(0);
        _size--;
    }

    return (char*)_arr;
}

String &String::format(const char *const format, ...) {
    va_list args;
	va_start(args, format);

	_size = Format::vformat((char*)_arr, _capacity, format, args); //格式化字符串转字符串

	va_end(args);
    return *this;
}

String &String::append(const String &str) {
    for(size_t i = 0; i < str.size(); i++) { //一个一个push
        push(str[i]);
    }
    
    return *this;
}

String &String::append(const char *const str) {
    size_t size = 0;
    while(str[size] != '\0') size++; //计算字符串长度

    for(size_t i = 0; i < size; i++) { //一个一个push
        push(str[i]);
    }

    return *this;
}

String &String::append(const char c) {
    push((uint8_t)c);

    return *this;
}