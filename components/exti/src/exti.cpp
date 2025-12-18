#include "exti.h"

const IRQn_Type Exti::__channels[16] = {
    EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn, 
    EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, 
    EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn
};

const uint32_t Exti::__afLines[16] = {
    LL_GPIO_AF_EXTI_LINE0, LL_GPIO_AF_EXTI_LINE1, LL_GPIO_AF_EXTI_LINE2, LL_GPIO_AF_EXTI_LINE3,
    LL_GPIO_AF_EXTI_LINE4, LL_GPIO_AF_EXTI_LINE5, LL_GPIO_AF_EXTI_LINE6, LL_GPIO_AF_EXTI_LINE7,
    LL_GPIO_AF_EXTI_LINE8, LL_GPIO_AF_EXTI_LINE9, LL_GPIO_AF_EXTI_LINE10, LL_GPIO_AF_EXTI_LINE11,
    LL_GPIO_AF_EXTI_LINE12, LL_GPIO_AF_EXTI_LINE13, LL_GPIO_AF_EXTI_LINE14, LL_GPIO_AF_EXTI_LINE15,
};

Exti* Exti::__extis[16] = {nullptr};

Exti::Exti() {
    this->__triggeredCallback = nullptr;
    this->__args = nullptr;
}

Exti::Exti(uint16_t id, uint16_t mode, uint8_t trigger, uint8_t priority, void (*cb)(void*), void* args) {
    init(id, mode, trigger, priority, cb, args);
}

Exti::~Exti() {
}

void Exti::init(uint16_t id, uint16_t mode, uint8_t trigger, uint8_t priority, void (*cb)(void*), void* args) {
    io.init(id, mode);
    uint8_t port = id >> 4;
    uint8_t pin = id & 0x000F;
    this->trigger = trigger;
    this->line =  LL_EXTI_LINE_0 << pin;
    this->channelIrq = __channels[pin];
    this->__triggeredCallback = nullptr;
    this->__args = nullptr;
    setTriggeredCallback(cb, args);

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO); //初始化AFIO（复用功能IO）

    LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTA + port, __afLines[pin]); //选择引脚通过AFIO连接到EXTI

    LL_EXTI_InitTypeDef stru;
    stru.Line_0_31 = line; //使用这一条
    stru.LineCommand = ENABLE; //开启
    stru.Mode = LL_EXTI_MODE_IT; //使用中断模式
    stru.Trigger = trigger; //触发模式
    LL_EXTI_Init(&stru);

    LL_EXTI_DisableIT_0_31(line); //先失能中断

    NVIC_SetPriority(channelIrq, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), priority, 0)); //配置通道中断
    NVIC_EnableIRQ(channelIrq); //使能通道

    __extis[pin] = this; //记录
}

void Exti::setTriggeredCallback(void (*cb)(void *), void *args) {
    __triggeredCallback = cb;
    __args = args;
}

void Exti::setPriority(uint8_t priority) {
    NVIC_SetPriority(channelIrq, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), priority, 0)); //配置通道中断
}

void Exti::start() {
    LL_EXTI_EnableIT_0_31(line); //使能中断
}

void Exti::start(void (*cb)(void *), void *args) {
    setTriggeredCallback(cb, args);
    start();
}

void Exti::stop() {
    LL_EXTI_DisableIT_0_31(line); //失能中断
}

Io *Exti::getIo() {
    return &io;
}

//中断函数
extern "C" void EXTI0_IRQHandler() {
    Exti* exti = Exti::__extis[0];
	if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args);
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
}
extern "C" void EXTI1_IRQHandler() {
    Exti* exti = Exti::__extis[1];
	if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args);
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
}
extern "C" void EXTI2_IRQHandler() {
    Exti* exti = Exti::__extis[2];
	if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args);
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
}
extern "C" void EXTI3_IRQHandler() {
    Exti* exti = Exti::__extis[3];
	if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args);
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
}
extern "C" void EXTI4_IRQHandler() {
    Exti* exti = Exti::__extis[4];
	if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args);
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_4);
}

extern "C" void EXTI9_5_IRQHandler() {
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_5)) { //判断是否是这个中断
        Exti* exti = Exti::__extis[5];
        if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args); //调用回调
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_5); //清除此中断标志位，这样就不会一直进入中断函数跳不出来
	}
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_6)) {
        Exti* exti = Exti::__extis[6];
        if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args);
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_6);
	}
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_7)) {
        Exti* exti = Exti::__extis[7];
        if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args);
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_7);
	}
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_8)) {
        Exti* exti = Exti::__extis[8];
        if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args);
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_8);
	}
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_9)) {
        Exti* exti = Exti::__extis[9];
        if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args);
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_9);
	}
}

extern "C" void EXTI15_10_IRQHandler() {
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_10)) {
        Exti* exti = Exti::__extis[10];
        if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args);
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_10);
	}
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_11)) {
        Exti* exti = Exti::__extis[11];
        if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args);
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_11);
	}
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_12)) {
        Exti* exti = Exti::__extis[12];
        if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args);
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_12);
	}
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_13)) {
        Exti* exti = Exti::__extis[13];
        if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args);
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);
	}
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_14)) {
        Exti* exti = Exti::__extis[14];
        if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args);
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_14);
	}
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_15)) {
        Exti* exti = Exti::__extis[15];
        if(exti && exti->__triggeredCallback) exti->__triggeredCallback(exti->__args);
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_15);
	}
}