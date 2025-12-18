摇杆类

依赖项：`io`, `adc`

示例：

```c++
Adc* adc = new Adc(1); //创建ADC，使用ADC1

Joystick* stick = new Joystick(adc, 1, PA1, adc, 0, PA0); //创建摇杆，其x，y轴的数据分别使用ADC的通道1，通道0及其绑定的IO口读取

JoystickStatus status = stick->get(); //获取摇杆数据
float val1 = status.x; //获取x轴上的数据（-1 到 1）
float val2 = status.y; //获取y轴上的数据（-1 到 1）
```