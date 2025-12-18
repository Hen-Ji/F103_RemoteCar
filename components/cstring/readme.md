字符串类，继承自 `Array<uint8_t>`，为字符串操作提供便利。

依赖项：`array`

示例：

```c++
String string(32); //创建容量为32的字符串

string.append("word"); //拼接字符串
string.append("world");

string += "hello"; //也可使用+=号拼接

string.format("number: %d", 666); //使用格式化字符串

char* str = string.string(); //返回C风格的字符串
```

若需要开启动态扩容，请在 array.h 中，将 QUEUE_USE_DYNAMIC 设为 true。

