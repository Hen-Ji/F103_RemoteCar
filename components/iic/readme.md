IIC类库，提供IIC驱动。

依赖项：

- `iic`: `io`, `os`, `osMutex`, `array`
- `iicSoftware`: `iic`
- `iicHardware`:  `iic`
- `iicHardwareIsr`: `iic`, `dma`

示例：

```c++
IicBus* iicBus = new IicBusSoftware(PA11, PA12, 1); //创建软件IIC总线，SCL引脚为PA11，SDA引脚为PA12，延时周期为1

IicDevice* device = new IicDevice(iicBus, 0x3C); //创建IIC设备，挂载在此IIC总线上，设备地址为0x3C（设备的7位地址应放在低7位，而不是高7位）

if(device->lock(1000)) { //请求独占总线使用权，等待时间为1000tick
    device->start(IicBus::Write); //生成起始信号，方向为写入方向

	device->transmit({0x00, 0x01}); //使用初始化列表发送数据
	device->transmit(dataArray, dataSize); //也可使用C语言常用的数组+长度的方式

	device->stop(); //生成停止信号
   	device->unlock(); //放弃总线使用权
}
```

注：`iic`只是一个基类，不提供驱动的实现。创建IIC总线时需要调用`iicSoftware`，`iicHardware`，`iicHardwareIsr` 的其中一个。

