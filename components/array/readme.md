一种实现了不定长数组，并可以选择是否在push时动态扩容的类。

依赖项：`os`

示例：

```c++
Array<uint8_t> array(10); //创建长度为10的数组
Array<int> arr2({1, 2, 3, 4, 5}); //以初始化列表的形式创建数据

array[0] = 123; //赋值操作
uint8_t val = array[2]; //取值操作

for(size_t i = 0; i < array.size(); i++) { //遍历此数组
	array[i] = i + 1;
}

array.resize(0); //改变数组长度
array.push(233); //在数据末尾添加元素
```

在 array.h 中，可将 ARRAY_USE_DYNAMIC 设为 true 开启动态扩容功能。