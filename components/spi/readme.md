SPI类库，提供SPI驱动。

依赖项：

- `spi`: `io`, `os`, `osMutex`, `array`
- `spiSoftware`: `spi`
- `spiHardware`:  `spi`

示例：

```c++
SpiBusHardware* spiBus = new SpiBusHardware(PA6, PA7, PA5, 1, 9000000); //创建硬件SPI，miso: PA6, mosi: PA7, sclk: PA5, 使用SPI1，速率为 9Mb/s

SpiDevice* device = new SpiDevice(spiBus, PA4); //创建SPI设备，使用此SPI，cs引脚为PA4

if(device->lock(1000)) { //请求独占总线使用权，等待时间为1000tick
    device->start(); //拉低CS引脚，开始传输

	device->transmit({0x00, 0x01}); //使用初始化列表发送数据
	device->transmit(dataArray, dataSize); //也可使用C语言常用的数组+长度的方式

	device->stop(); //拉高CS引脚，结束传输
   	device->unlock(); //放弃总线使用权
}
```

注：`spi` 只是一个基类，不提供驱动的实现。创建SPI总线时需要调用 `spiSoftware`，`spiHardware` 的其中一个。

