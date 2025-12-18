LoRa模块类，芯片使用LLCC68

依赖项：`os`, `spi`, `array`, `io`

示例：

```c++
void loraTransmitTask(void *args) { //发送任务
    spiBus = new SpiBusHardware(PA6, PA7, PA5, 1, 9000000); //创建SPI总线
    Lora* lora = new Lora(spiBus, PA4, PA8, PA10, PA9); //创建lora模块，使用此SPI总线，cs: PA4, rst: PA8, busy: PA10, dio: PA9
    String* buffer = new String(128); //创建字符串
    lora->setTransmitMode(); //lora模块设置为发送模式

    while(1) {
        lora->transmit(buffer->format("test: %d", cnt)); //发送字符串
        Os::delay(500); //延时500tick
    }
}

void loraReceiveTask(void *args) { //接收任务
    spiBus = new SpiBusHardware(PA6, PA7, PA5, 1, 9000000); //创建SPI总线
    Lora* lora = new Lora(spiBus, PA4, PA8, PA10, PA9); //创建lora模块，使用此SPI总线，cs: PA4, rst: PA8, busy: PA10, dio: PA9
    String* buffer = new String(128); //创建字符串
    lora->setReceiveMode(); //lora模块设置为接收模式

    while(1) {
        lora->receive(buffer, -1); //接收数据到buffer中，一直等待
        Log::info("lora received: %s", buffer->string()); //输出buffer
        Os::delay(50); //延时50tick
    }
}
```

注：此LoRa类不能同时发送和接收