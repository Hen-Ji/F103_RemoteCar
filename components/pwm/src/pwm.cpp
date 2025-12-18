#include "pwm.h"

TIM_TypeDef* const Pwm::__tims[] = {0, TIM2, TIM3, TIM4};
uint32_t Pwm::__channels[] = {LL_TIM_CHANNEL_CH1, LL_TIM_CHANNEL_CH2, LL_TIM_CHANNEL_CH3, LL_TIM_CHANNEL_CH4};

Pwm::Pwm() {
}

Pwm::Pwm(Timer *timer, uint16_t io, uint8_t channelId, uint32_t compare) {
    init(timer, io, channelId, compare);
}

Pwm::~Pwm() {
}

void Pwm::init(Timer *timer, uint16_t io, uint8_t channelId, uint32_t compare) {
    this->timer = timer;
    this->handle = __tims[timer->getId()-1];
    this->channelId = channelId;
    this->channel = __channels[channelId-1];
    this->io.init(io, Io::Output | Io::PushPull | Io::Alternative); //复用推挽输出

    LL_TIM_OC_EnablePreload(handle, channel);

    LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
    TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1; //使用PWM模式1(计时器在使用向上计数模式时，CNT(计时器的计数) < CCR(比较寄存器) 时，REF置有效电平；CNT >= CCR 时，REF置无效电平)
    TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_ENABLE; //比较输出使能
    TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE; //N模式的比较输出，失能
    TIM_OC_InitStruct.CompareValue = compare; //设置CCR的值
    TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH; //有效电平为高电平
    LL_TIM_OC_Init(handle, channel, &TIM_OC_InitStruct);

    LL_TIM_OC_EnableFast(handle, channel); //使用快速模式
    LL_TIM_SetTriggerOutput(handle, LL_TIM_TRGO_RESET); //不使用TRGO
}

void Pwm::start() {
    LL_TIM_CC_EnableChannel(handle, channel); //使能通道
}

void Pwm::start(uint32_t compare) {
    setCompare(compare);
    start();
}

void Pwm::stop() {
    LL_TIM_CC_DisableChannel(handle, channel); //禁止通道
}

void Pwm::setDuty(float duty) {
    setCompare(duty * (timer->getReload()+1));
}

void Pwm::setCompare(uint32_t compare) {
    this->compare = compare;
    if(channelId == 1) LL_TIM_OC_SetCompareCH1(handle, compare); //根据通道选用不同的函数
    else if(channelId == 2) LL_TIM_OC_SetCompareCH2(handle, compare);
    else if(channelId == 3) LL_TIM_OC_SetCompareCH3(handle, compare);
    else if(channelId == 4) LL_TIM_OC_SetCompareCH4(handle, compare);
}
