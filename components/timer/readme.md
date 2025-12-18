定时器类

无依赖项

示例：

```c++
Timer* timer = new Timer(4, 20000 - 1, 72 - 1, callback); //创建定时器，使用定时器4，重载值为20000 - 1，预分频器的值为72 - 1，对于外设时钟为72MHz的单片机来说，此定时器的溢出周期为20ms，设置溢出(超时)回调函数为callback

timer->start(); //启动定时器

void callback(void* args) { //定时器溢出回调
    ... //事件处理
}
```