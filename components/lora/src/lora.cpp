#include "lora.h"

void Lora::transmitCommands(uint8_t *commands, size_t size) {
    if(device.lock(-1)) {
        for(size_t i = 0; i < size; i++) {
            size_t s = commands[i];
            if(s == 0) break;

            while(isBusy()); //等待设备空闲
            device.start(); 
            device.transmit(&commands[i+1], s); //传输
            device.stop();
            for(volatile uint16_t j = 0; j < 100; j++); //需要延时至少600ns，让设备反应

            //i += s + 1; //我要把这行代码钉在我的床头TAT
            i += s;
        }
        device.unlock();
    }
}

void Lora::swapBytes(uint8_t *data, uint8_t *receiveBuffer, uint32_t size) {
    if(device.lock(-1)) {
        while(isBusy()); //等待设备空闲

        device.start(); 
        device.swap(data, receiveBuffer, size); //交换
        device.stop();

        for(volatile uint16_t j = 0; j < 100; j++); //需要延时至少600ns，让设备反应
        device.unlock();
    }
}

void Lora::transmitBytes(uint8_t *bytes, uint16_t size) {
    if(device.lock(-1)) {
        while(isBusy()); //等待设备空闲

        device.start(); 
        device.transmit(bytes, size); //传输
        device.stop();

        for(volatile uint16_t j = 0; j < 100; j++); //需要延时至少600ns，让设备反应
        device.unlock();
    }
}

Lora::Lora() {
}

Lora::Lora(SpiBus *bus, uint16_t cs, uint16_t rst, uint16_t busy, uint16_t dio, uint32_t freq, uint8_t bandwidth, uint8_t spreadingFactor, uint8_t codingRate) {
    init(bus, cs, rst, busy, dio, freq, bandwidth, spreadingFactor, codingRate);
}

Lora::~Lora() {
}

void Lora::init(SpiBus *bus, uint16_t cs, uint16_t rst, uint16_t busy, uint16_t dio, uint32_t freq, uint8_t bandwidth, uint8_t spreadingFactor, uint8_t codingRate) {
    mutex.init();
    this->transmitting = false;
    this->transmitTask = nullptr;

    device.init(bus, cs); //初始化SPI设备
    this->rst.init(rst, Io::Output | Io::PushPull); //初始化引脚
    this->busy.init(busy, Io::Input | Io::Float);
    this->dio.init(dio, Io::Input | Io::Float);

    this->freq = freq; //初始化参数
    this->bandwidth = bandwidth;
    this->spreadingFactor = spreadingFactor;
    this->codingRate = codingRate;

    this->rst.setLow(); //初始化LLCC68
    Os::delayMs(30);
    this->rst.setHigh();
    Os::delayMs(20);

    setStandbyMode(); //设置standby
}

void Lora::setFreq(uint32_t freq) {
    this->freq = freq;
}

void Lora::setBandwidth(uint8_t bw) {
    this->bandwidth = bw;
}

void Lora::setSpreadingFactor(uint8_t sf) {
    this->spreadingFactor = sf;
}

void Lora::setCodingRate(uint8_t cr) {
    this->codingRate = cr;
}

void Lora::setMode(uint8_t mode) {
    if(mode == Mode::Standby) setStandbyMode();
    else if(mode == Mode::Tx) setTransmitMode();
    else if(mode == Mode::Rx) setReceiveMode();
}

void Lora::setStandbyMode() {
    uint8_t datas[] = {0x80, 0x00}; //设置STDBY_RC
    transmitBytes(datas, sizeof(datas));
    mode = Mode::Standby;
}

void Lora::setTransmitMode() {
    setStandbyMode();
	
	uint32_t freq = (uint32_t)((uint64_t)this->freq * (1 << 25) / 32000000); //计算射频频率
	uint8_t txClampConfig = readRegister(0x08D8);
	txClampConfig |= 0x1E; // fix bug: 15.2 Better Resistance of the LLCC68 Tx to Antenna Mismatch
	uint8_t initParams[] = {
		2, 0x8A, 0x01, // 设置为Lora模式
		5, 0x0D, 0x07, 0x40, 0x34, 0x44, // 作为公共网络(0x3444)设置同步字
		2, 0x96, 0x01, // 使用DC-DC
		5, 0x86, (uint8_t)(freq >> 24), (uint8_t)((freq >> 16) % 0xFF), (uint8_t)((freq >> 8) % 0xFF), (uint8_t)(freq % 0xFF), // 设置射频频率
		5, 0x95, 0x04, 0x07, 0x00, 0x01, // 设置放大器功率为+22dBm（最大值）
		3, 0x8E, 22, 0x02, // 设置TxParams：设置输出功率为+22dBm，斜坡时间为40us（0x02）
		4, 0x0D, 0x08, 0xD8, txClampConfig, // 优化放大器阈值，以达到期望的功率
		3, 0x8F, 0x00, 0x00, // 设置Tx和Rx的基地址均为0
		5, 0x8B, // 设置modulationParams
			spreadingFactor, // 设置扩散因子
			bandwidth, // 设置带宽
			codingRate, // 设置编码率
			0x00, // 关闭低速率优化(0x00: off; 0x01: on)
    	9, 0x08, 0xFF, 0xFF, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, // 开启所有中断，并将Timeout和TxDone中断映射到DIO1上
		0
	};
  	transmitCommands(initParams, sizeof(initParams));
    
    mode = Mode::Tx;
}

void Lora::setReceiveMode() {
    setStandbyMode();
	
	uint32_t freq = (uint32_t)((uint64_t)this->freq * (1 << 25) / 32000000); //计算射频频率
	uint8_t reg = readRegister(0x0736);
	reg |= 0x04; // fix bug: 15.4 Optimizing the Inverted IQ Operation
	uint8_t initParams[] = {
		2, 0x8A, 0x01, // 设置为Lora模式
		5, 0x0D, 0x07, 0x40, 0x34, 0x44, // 作为公共网络(0x3444)设置同步字
		2, 0x96, 0x01, // 使用DC-DC
		5, 0x86, (uint8_t)(freq >> 24), (uint8_t)((freq >> 16) % 0xFF), (uint8_t)((freq >> 8) % 0xFF), (uint8_t)(freq % 0xFF), // 设置射频频率
		3, 0x8F, 0x00, 0x00, // 设置Tx和Rx的基地址均为0
		5, 0x8B, // 设置modulationParams
			spreadingFactor, // 设置扩散因子
			bandwidth, // 设置带宽
			codingRate, // 设置编码率
			0x00, // 关闭低速率优化(0x00: off; 0x01: on)
    	7, 0x8C, // 设置 packetParams
    		0x00, 0x08, // 设置前导码长度为 8
    	  	0x00, // 设置为不定长数据表
      		0xFF, // 设置有效载荷长度为255（最大值）
      		0x01, // 使用CRC校验
      		0x00, // 使用标准IQ设置
		4, 0x0D, 0x07, 0x36, reg, // bit 2 must be 1 when using standard IQ polarity
    	9, 0x08, 0xFF, 0xFF, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, // 开启所有中断，并将Timeout和RxDone中断映射到DIO1上
		4, 0x82, 0xFF, 0xFF, 0xFF, // 启动Rx模式并设置超时时间，0xFFFFFF表示一直接收
		0
	};
  	transmitCommands(initParams, sizeof(initParams));

    mode = Mode::Rx;
}

bool Lora::transmit(const uint8_t *data, size_t size, int32_t timeout) {
    if(mode == Mode::Tx && mutex.lock(timeout)) { //上锁
        transmitting = true;
        if(device.lock(timeout)) {
            while(isBusy()); //等待设备空闲

            device.start(); 
            device.transmit({0x0E, 0x00}); // 写入缓冲指令，设置偏移量为0
            device.transmit(data, size); //传输
            device.stop();

            for(volatile uint16_t j = 0; j < 100; j++); //需要延时至少600ns，让设备反应
            device.unlock();
        }
        else return false;

        //uint32_t t = (float)1000 * timeout / 15.625; // 计算发送超时时间
        uint32_t t = 0; // 一直发送
        uint8_t params[] = {
            7, 0x8C, // 设置 packetParams
                0x00, 0x08, // 设置前导码长度为 8
                0x00, // 设置为不定长数据表
                (uint8_t)size, // 设置有效载荷长度
                0x01, // 使用CRC校验
                0x00, // 使用标准IQ设置
            4, 0x83, (uint8_t)((t >> 16) % 0xFF), (uint8_t)((t >> 8) % 0xFF), (uint8_t)(t % 0xFF), // 启动Tx模式并设置超时时间，0x000000表示一直发送，直到发送完成
            0
        };
        transmitCommands(params, sizeof(params)); //将数据发送给设备，准备传输

        Os::delay(1); //等待数据发送完成
        while(dio.get() != 1) {
            Os::delay(1);
        }

        bool res = false;
        LoraIrqStatus status = getIrqStatus(); //获取irq标志
        clearIrqStatus(0xFFFF); //清除所有irq标志
        if(mode == Mode::Tx) { //发送模式
            transmitting = false;
            if(status.txDone) res = true; //发送成功
            if(status.timeout) res = false; //发送超时
        }

        mutex.unlock();
        return res;
    }
    else return false;

    return true;
}

bool Lora::transmit(const std::initializer_list<uint8_t> &data, int32_t timeout) {
    return transmit(data.begin(), data.size(), timeout);
}

bool Lora::transmit(const Array<uint8_t> &data, int32_t timeout) {
    return transmit(data.array(), data.size(), timeout);
}

bool Lora::transmit(const Array<uint8_t> *data, int32_t timeout) {
    return transmit(data->array(), data->size(), timeout);;
}

bool Lora::receive(uint8_t *buffer, size_t *size, int32_t timeout) {
    uint32_t tick = timeout == -1 ? ~0UL : (timeout + Os::getCurrentTick());
    
    while(tick > Os::getCurrentTick() && (dio.get() != 1 || mode != Mode::Rx)) { //等待有数据接收
        Os::delay(1);
    }
    if(dio.get() == 1 && mode == Mode::Rx && mutex.lock(-1)) {
        LoraIrqStatus status = getIrqStatus(); //获取irq标志
        clearIrqStatus(0xFFFF); //清除所有irq标志

        if(status.rxDone && !status.crcErr && !status.headerErr) { //接收成功
            uint8_t data[4] = {0x13, 0, 0, 0};
            uint8_t bufferStatus[4] = {0};
            swapBytes(data, bufferStatus, 4); // 获取RxBufferStatus，bufferStatus[2]为接收到的数据长度，bufferStatus[3]为获取接收到的数据在Lora模块的buffer中的起始位置
            *size = bufferStatus[2]; //获取长度

            if(device.lock(-1)) {
                while(isBusy()); //等待设备空闲

                device.start(); 
                device.transmit({0x1E, bufferStatus[3], 0x00});
                device.receive(buffer, bufferStatus[2]); //传输
                device.stop();

                for(volatile uint16_t j = 0; j < 100; j++); //需要延时至少600ns，让设备反应
                device.unlock();
            }
        }

        mutex.unlock();
        return true;
    }
    return false;
}

bool Lora::receive(Array<uint8_t> &data, int32_t timeout) {
    size_t size = 0;
    if(receive(data.array(), &size, timeout)) {
        data.resize(size);
        return true;
    }
    return false;
}

bool Lora::receive(Array<uint8_t> *data, int32_t timeout) {
    size_t size = 0;
    if(receive(data->array(), &size, timeout)) {
        data->resize(size);
        return true;
    }
    return false;
}

bool Lora::isBusy()
{
    return busy.get();
}

uint8_t Lora::getDio() {
    return dio.get();
}

bool Lora::isTransmitting() {
    return transmitting;
}

LoraStatus Lora::getStatus() {
    LoraStatus res;
    uint8_t data[2] = {0xC0};
	uint8_t buf[2] = {0};
	swapBytes(data, buf, 2); //获取状态码
	
    res.mode = (buf[1] & 0x70) >> 4; //获取当前模式
	res.mode = (buf[1] & 0x0E) >> 1; //获取当前状态
    return res;
}

LoraIrqStatus Lora::getIrqStatus() {
    LoraIrqStatus res;
    uint8_t data[4] = {0x12};
	uint8_t buf[4] = {0};
	swapBytes(data, buf, 4); //获取状态码

    res.txDone = buf[3] & 0x01; //获取中断状态
    res.rxDone = (buf[3] >> 1) & 0x01;
    res.preambleDetected = (buf[3] >> 2) & 0x01;
    res.headerValid = (buf[3] >> 4) & 0x01;
    res.headerErr = (buf[3] >> 5) & 0x01;
    res.crcErr = (buf[3] >> 6) & 0x01;
    res.cadDone = buf[3] >> 7;
    res.cadDetected = buf[2] & 0x01;
    res.timeout = (buf[2] >> 1) & 0x01;
    return res;
}

void Lora::clearIrqStatus(uint16_t mask) {
	uint8_t data[] = {0x02, (uint8_t)(mask >> 8), (uint8_t)(mask & 0xFF)};
	transmitBytes(data, sizeof(data));
}

uint8_t Lora::readRegister(uint16_t addr) {
	uint8_t data[5] = {0x1D, (uint8_t)(addr >> 8), (uint8_t)(addr & 0xFF)};
	uint8_t buf[5] = {0};

	swapBytes(data, buf, 5);
	return buf[4];
}
