#include "timer.h"

TIM_TypeDef* const Timer::__tims[] = {0, TIM2, TIM3, TIM4};
const uint32_t Timer::__periphs[] = {0, LL_APB1_GRP1_PERIPH_TIM2, LL_APB1_GRP1_PERIPH_TIM3, LL_APB1_GRP1_PERIPH_TIM4};
const IRQn_Type Timer::__channelIrqs[] = {(IRQn_Type)0, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn};

Timer* Timer::__timers[] = {nullptr, nullptr, nullptr, nullptr};

Timer::Timer() {
}

Timer::Timer(uint8_t id, uint16_t reload, uint16_t prescaler, uint8_t priority, void (*cb)(void *), void *args) {
    init(id, reload, prescaler, priority, cb, args);
}

Timer::~Timer() {
}

void Timer::init(uint8_t id, uint16_t reload, uint16_t prescaler, uint8_t priority, void (*cb)(void *), void *args) {
    this->id = id;
    this->reload = reload;
    this->prescaler = prescaler;
    this->channelIrq = __channelIrqs[id-1];
    this->priority = priority;
    this->__timeoutCallback = cb;
    this->__args = args;
    this->handle = __tims[id-1];
    this->output = false;

    LL_APB1_GRP1_EnableClock(__periphs[id-1]); //使能时钟，TIM2,3,4都在APB1上，TIM1在APB2上

    LL_TIM_InitTypeDef TIM_InitStruct = {0};

    TIM_InitStruct.Prescaler = prescaler; //预分频器的值
    TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP; //向上计数
    TIM_InitStruct.Autoreload = reload; //自动重载值
    TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1; //不分频
    LL_TIM_Init(handle, &TIM_InitStruct);
    LL_TIM_EnableARRPreload(handle); //开启自动重载
    LL_TIM_SetClockSource(handle, LL_TIM_CLOCKSOURCE_INTERNAL); //使用内部时钟

    NVIC_SetPriority(channelIrq, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),priority, 0)); //配置中断优先级
    NVIC_EnableIRQ(channelIrq); //使能

    LL_TIM_DisableIT_UPDATE(handle); //禁止超时中断

    __timers[id-1] = this;
}

void Timer::start() {
    if(__timeoutCallback) LL_TIM_EnableIT_UPDATE(handle); //启动超时中断
    LL_TIM_EnableCounter(handle); //启动计时器
}

void Timer::start(void (*cb)(void *), void *args) {
    setTimeoutCallback(cb, args);
    start();
}

void Timer::stop() {
    LL_TIM_DisableCounter(handle); //停止计时器
    LL_TIM_DisableIT_UPDATE(handle); //禁止超时中断
}

void Timer::setPriority(uint8_t priority) {
    NVIC_SetPriority(channelIrq, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),priority, 0)); //配置中断优先级
}

void Timer::setTimeoutCallback(void (*cb)(void *), void *args) {
    __timeoutCallback = cb;
    __args = args;
}

uint32_t Timer::getReload() {
    return reload;
}

uint8_t Timer::getId() {
    return id;
}

void Timer::enableOutput() {
    output = true;
    LL_TIM_EnableAllOutputs(handle); //使能输出
}

void Timer::disableOutput() {
    output = false;
    LL_TIM_DisableAllOutputs(handle); //失能输出
}

//中断函数
extern "C" void TIM2_IRQHandler(void) {
    Timer* timer = Timer::__timers[1];
	if(LL_TIM_IsActiveFlag_UPDATE(TIM2)) { //判断是否是此计时器更新时产生的中断
		if(timer && timer->__timeoutCallback) timer->__timeoutCallback(timer->__args); //调用回调
        LL_TIM_ClearFlag_UPDATE(TIM2); //清除中断标志位
	}
}
extern "C" void TIM3_IRQHandler(void) {
    Timer* timer = Timer::__timers[2];
	if(LL_TIM_IsActiveFlag_UPDATE(TIM3)) {
		if(timer && timer->__timeoutCallback) timer->__timeoutCallback(timer->__args);
        LL_TIM_ClearFlag_UPDATE(TIM3);
	}
}
extern "C" void TIM4_IRQHandler(void) {
    Timer* timer = Timer::__timers[3];
	if(LL_TIM_IsActiveFlag_UPDATE(TIM4)) {
		if(timer && timer->__timeoutCallback) timer->__timeoutCallback(timer->__args);
        LL_TIM_ClearFlag_UPDATE(TIM4);
	}
}
