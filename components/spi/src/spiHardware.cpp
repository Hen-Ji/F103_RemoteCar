#include "spiHardware.h"

SpiBusHardware::SpiBusHardware() {
    freq = 0;
}

SpiBusHardware::SpiBusHardware(uint16_t miso, uint16_t mosi, uint16_t sclk, uint8_t id, uint32_t freq) {
    init(miso, mosi, sclk, id, freq);
}

SpiBusHardware::~SpiBusHardware() {
}

void SpiBusHardware::init(uint16_t miso, uint16_t mosi, uint16_t sclk, uint8_t id, uint32_t freq) {
    this->mutex.init();
    this->freq = freq;
    this->id = id;

    uint32_t bauds;
    int32_t x = freq == 0 ? 0 : CLOCK_FREQ / freq; //计算分频, 为0默认跑满速率
    if(id == 2) x /= 2; //SPI2挂载在APB1，其时钟频率比APB1的频率少一半，要达到相同的速率分频需要除2
    
    if(x <= 2) bauds = LL_SPI_BAUDRATEPRESCALER_DIV2; //计算分频
    else if(x <= 4) bauds = LL_SPI_BAUDRATEPRESCALER_DIV4;
    else if(x <= 8) bauds = LL_SPI_BAUDRATEPRESCALER_DIV8;
    else if(x <= 16) bauds = LL_SPI_BAUDRATEPRESCALER_DIV16;
    else if(x <= 32) bauds = LL_SPI_BAUDRATEPRESCALER_DIV32;
    else if(x <= 64) bauds = LL_SPI_BAUDRATEPRESCALER_DIV64;
    else if(x <= 128) bauds = LL_SPI_BAUDRATEPRESCALER_DIV128;
    else bauds = LL_SPI_BAUDRATEPRESCALER_DIV256;

    if(id == 1) { //SPI1
        this->miso.init(miso, Io::Input | Io::PushUp); //miso上拉输入
        this->mosi.init(mosi, Io::Output | Io::PushPull | Io::Alternative); //mosi复用推挽输出
        this->sclk.init(sclk, Io::Output | Io::PushPull | Io::Alternative); //sclk复用推挽输出
        this->handle = SPI1;

        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1); //SPI1在APB2上, SPI2在APB1上
    }
    else if(id == 2) { //SPI2
        this->miso.init(miso, Io::Input | Io::PushUp);
        this->mosi.init(mosi, Io::Output | Io::PushPull | Io::Alternative);
        this->sclk.init(sclk, Io::Output | Io::PushPull | Io::Alternative);
        this->handle = SPI2;

        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
    }

    LL_SPI_InitTypeDef SPI_InitStruct = {0};
    SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX; //双线全双工
    SPI_InitStruct.Mode = LL_SPI_MODE_MASTER; //主模式
    SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT; //8位数据帧
    SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW; //时钟极性为默认低电平
    SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE; //第一个边沿采样（上升沿时获取），CPOL和CPHA共同组成模式0
    SPI_InitStruct.NSS = LL_SPI_NSS_SOFT; //软件片选
    SPI_InitStruct.BaudRate = bauds; //分频
    SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST; //高位先行
    SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE; //不使用CRC校验
    SPI_InitStruct.CRCPoly = 10; //CRC校验数
    LL_SPI_Init(SPI1, &SPI_InitStruct);
    LL_SPI_Enable(handle);
}

void SpiBusHardware::swap(const uint8_t *data, uint8_t *buffer, const size_t size, const uint8_t fill) {
    for(size_t i = 0; i < size; i++) {
        while(!LL_SPI_IsActiveFlag_TXE(handle)); //等待寄存器TX为空
        LL_SPI_TransmitData8(handle, data ? data[i] : fill); //写数据
        while(LL_SPI_IsActiveFlag_BSY(handle)); //等待传输完成
        while(!LL_SPI_IsActiveFlag_RXNE(handle)); //等待寄存器RX非空，即收到数据
        if(buffer) buffer[i] = LL_SPI_ReceiveData8(handle); //读数据返回
        else LL_SPI_ReceiveData8(handle); //不需要读数据也要清空RX寄存器，否则会读到上一次的数据，接收不到需要的数据
    }
}