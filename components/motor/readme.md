直流电机驱动类

依赖项：`timer`, `pwm`

示例：

```c++
Timer* motorTimer = new Timer(2, 7200 - 1, 0); //创建定时器，周期0.1ms，即频率10kHz
Motor* motor = new Motor(motorTimer, 3, PA2, motorTimer, 4, PA3); //创建电机模块，正极使用定时器motorTimer通道3的PWM输出，父级使用定时器motorTimer通道4的PWM输出，并绑定对应通道的引脚

motorTimer->enableOutput(); //使能定时器的输出
motor->start(); //启动电机
motorTimer->start(); //启动定时器

motor->setSpeed(0.8); //设置电机速度，正数为正转，负数为反转，0不转，1或-1为最高转速
```