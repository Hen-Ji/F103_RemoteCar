一种实现了环形队列的类。

依赖项：`os`

示例：

```c++
Queue<uint8_t> queue(10); //创建容量为10的队列

queue.push(1); //元素入队
queue.push(2);
queue.push(3);

uint8_t val = queue.pop(); //元素出队

val = queue.front(); //队头元素
val = queue.back(); //队尾元素
```

在 cqueue.h 中，可将 QUEUE_USE_DYNAMIC 设为 true 开启动态扩容功能。