OLED驱动类，使用SSD1306

依赖项：`iic`, `array`

示例：

```c++
IicBusSoftware* iicBus = new IicBusSoftware(PA11, PA12, 1); //初始化软件IIC, SCL -> PA11, SDA -> PA12，间隔时间为1
Oled* oled = new Oled(iicBus); //创建OLED，使用此IIC总线
String* buffer = new String(128); //创建字符串

oled->clear(); //清屏
oled->show(0, 1, buffer->format("hello world!")); //以(0, 1)为第一个字符的位置，打印字符串
oled->update(); //刷新
```