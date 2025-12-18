#include "adc.h"

const uint32_t AdcChannel::__channels[] = {
    LL_ADC_CHANNEL_0, LL_ADC_CHANNEL_1, LL_ADC_CHANNEL_2, LL_ADC_CHANNEL_3,
    LL_ADC_CHANNEL_4, LL_ADC_CHANNEL_5, LL_ADC_CHANNEL_6, LL_ADC_CHANNEL_7,
    LL_ADC_CHANNEL_8, LL_ADC_CHANNEL_9, LL_ADC_CHANNEL_10, LL_ADC_CHANNEL_11,
    LL_ADC_CHANNEL_12, LL_ADC_CHANNEL_13, LL_ADC_CHANNEL_14, LL_ADC_CHANNEL_15
};

Adc::Adc() {
}

Adc::Adc(uint8_t id) {
    init(id);
}

Adc::~Adc() {
}

void Adc::init(uint8_t id) {
    this->id = id;
    if(id == 1) {
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1); //开启ADC时钟
        handle = ADC1;
    }
    else if(id == 1) {
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC2);
        handle = ADC2;
    }

    LL_ADC_InitTypeDef ADC_InitStruct = {0};
    LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};
    LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
    
    ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT; //右对齐(从寄存器的最右边（最低位）开始存，这样就不用转换数值了)
    ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE; //使用非扫描模式
    LL_ADC_Init(handle, &ADC_InitStruct);
    ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT; //独立模式
    LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(handle), &ADC_CommonInitStruct);
    ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE; //不使用外部触发源，使用软件触发
    ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE; //规则组长度
    ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE; //非连续转换
    ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE; //单次转换
    ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE; //不使用DMA
    LL_ADC_REG_Init(handle, &ADC_REG_InitStruct);

    LL_ADC_Enable(handle); //使能ADC
	LL_ADC_StartCalibration(handle); //开始校准ADC
	while(LL_ADC_IsCalibrationOnGoing(handle)); //等待ADC校准完成
}

uint8_t Adc::getId() {
    return id;
}

AdcChannel::AdcChannel() {
}

AdcChannel::AdcChannel(Adc *adc, uint8_t channelId, uint16_t io) {
    init(adc, channelId, io);
}

AdcChannel::~AdcChannel() {
}

void AdcChannel::init(Adc *adc, uint8_t channelId, uint16_t io) {
    this->io.init(io, Io::Input | Io::Analog); //模拟输入
    this->channelId = channelId;
    this->channel = __channels[channelId];
    this->adc = adc;
    if(adc->getId() == 1) {
        handle = ADC1;
    }
    else if(adc->getId() == 1) {
        handle = ADC2;
    }

    LL_ADC_SetChannelSamplingTime(handle, channel, LL_ADC_SAMPLINGTIME_13CYCLES_5); //通道采样时间选择13.5个ADCCLK的时钟周期
}

uint16_t AdcChannel::get() {
    LL_ADC_REG_SetSequencerRanks(handle, LL_ADC_REG_RANK_1, channel); //放在第1个序列
    LL_ADC_REG_StartConversionSWStart(handle); //启动规则转换通道
	while(!LL_ADC_IsActiveFlag_EOS(handle));//等待转换结束	
    return LL_ADC_REG_ReadConversionData12(handle); //返回结果
}
