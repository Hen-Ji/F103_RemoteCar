IO口类

无依赖项

示例：

```c++
Io io = new Io(PC13, Io::Output | Io::PushPull); //创建IO口，引脚名称为PC13，初始化为推挽输出

io->set(1); //使用set函数设置高电平
io->setLow(); //使用setLow函数设置低电平（比io->set(0)性能高）
io->toggle(); //翻转电平
```

