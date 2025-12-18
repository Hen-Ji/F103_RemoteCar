ADC类

依赖项：`io`

示例：

```c++
Adc* adc = new Adc(1); //创建ADC，使用ADC1
AdcChannel* channel = new AdcChannel(adc, 1, PA1); //创建ADC通道，使用通道1，输入引脚为PA1

uint16_t val = channel->get(); //获取ADC的值，范围为0~4095
```