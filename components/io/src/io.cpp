#include "io.h"

GPIO_TypeDef * const Io::__ports[] = {GPIOA, GPIOB, GPIOC, GPIOD};
const uint32_t Io::__pins[] = {
    LL_GPIO_PIN_0, LL_GPIO_PIN_1, LL_GPIO_PIN_2, LL_GPIO_PIN_3, 
    LL_GPIO_PIN_4, LL_GPIO_PIN_5, LL_GPIO_PIN_6, LL_GPIO_PIN_7,
    LL_GPIO_PIN_8, LL_GPIO_PIN_9, LL_GPIO_PIN_10, LL_GPIO_PIN_11,
    LL_GPIO_PIN_12, LL_GPIO_PIN_13, LL_GPIO_PIN_14, LL_GPIO_PIN_15
};
const uint32_t Io::__periphs[] = {LL_APB2_GRP1_PERIPH_GPIOA, LL_APB2_GRP1_PERIPH_GPIOB, LL_APB2_GRP1_PERIPH_GPIOC, LL_APB2_GRP1_PERIPH_GPIOD};

Io::Io() {
    id = 0;
    port = nullptr;
    pin = 0;
    mode = 0;
}

Io::Io(uint16_t id, uint16_t mode) {
    init(id, mode);
}

Io::~Io() {
}

void Io::init(uint16_t id, uint16_t mode) {
    //初始化属性
    this->id = id;
    this->mode = mode;
    port = __ports[id >> 4];
    pin = __pins[id & 0x000F];

    //初始化时钟
    LL_APB2_GRP1_EnableClock(__periphs[id >> 4]);
    
    //初始化引脚电平
    LL_GPIO_ResetOutputPin(port, pin);

    //初始化结构体
    LL_GPIO_InitTypeDef stru = {0};
    stru.Pin = pin; //引脚

    if(mode & Mode::Alternative) stru.Mode = LL_GPIO_MODE_ALTERNATE; //复用模式
    else if(mode & Mode::Analog) stru.Mode = LL_GPIO_MODE_ANALOG; //模拟模式
    else if(mode & Mode::Float) stru.Mode = LL_GPIO_MODE_FLOATING; //浮空模式
    else if(mode & (Mode::Output | Mode::PushPull | Mode::OpenDrain)) stru.Mode = LL_GPIO_MODE_OUTPUT; //输出模式
    else if(mode & (Mode::Input | Mode::PushUp | Mode::PushDown)) stru.Mode = LL_GPIO_MODE_INPUT; //输出模式
    else stru.Mode = 0; //默认为0(模拟模式)

    if(mode & Mode::PushPull) stru.OutputType = LL_GPIO_OUTPUT_PUSHPULL; //推挽输出
    else if(mode & Mode::OpenDrain) stru.OutputType = LL_GPIO_OUTPUT_OPENDRAIN; //开漏输出
    else stru.OutputType = 0; //默认为0（推挽）

    if(mode & Mode::PushUp) stru.Pull = LL_GPIO_PULL_UP; //上拉输入
    else if(mode & Mode::PushDown) stru.Pull = LL_GPIO_PULL_DOWN; //下拉输入
    else stru.Pull = 0; //默认为0（下拉）

    stru.Speed = LL_GPIO_SPEED_FREQ_HIGH; //速度默认为高
    LL_GPIO_Init(port, &stru); //初始化
}
