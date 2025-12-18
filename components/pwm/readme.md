PWM类

依赖项：`timer`, `io`

示例：

```c++
Timer* timer = new Timer(2, 7200 - 1, 0); //创建一个定时器，频率10kHz
Pwm* pwm = new Pwm(timer, 3, PA2); //创建PWM，使用此定时器的通道3以及对应的PA2引脚

timer->enableOutput(); //使能定时器的输出
pwm->start(); //启动PWM
timer->start(); //启动timer

pwm->setDuty(0.5); //设置占空比
```