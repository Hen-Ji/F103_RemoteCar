#include "oled.h"
#include "io.h"
#include "oledFont.h"
#include "os.h"

const uint8_t oledInitData[] = {
	0xAE,	//关闭显示
	0xD5,	//设置显示时钟分频比/振荡器频率
	0x80,
	0xA8,	//设置多路复用率
	0x3F,
	0xD3,	//设置显示偏移
	0x00,
	0x40,	//设置显示开始行
	0xA1,	//设置左右方向，0xA1正常 0xA0左右反置
	0xC8,	//设置上下方向，0xC8正常 0xC0上下反置
	0xDA,	//设置COM引脚硬件配置
	0x12,
	0x81,	//设置对比度控制
	0xCF,
	0xD9,	//设置预充电周期
	0xF1,
	0xDB,	//设置VCOMH取消选择级别
	0x30,
	0xA4,	//设置整个显示打开/关闭
	0xA6,	//设置正常/倒转显示
	0x8D,	//设置充电泵
	0x14,
	0xAF	//开启显示
};

void Oled::sendCommand(const uint8_t data) {
	device.start(IicBus::Write);
	device.transmit({0x00, data});
	device.stop();
}

void Oled::sendCommand(const uint8_t *data, const uint8_t size) {
	device.start(IicBus::Write);
	device.transmit({0x00});
	device.transmit(data, size);
	device.stop();
}

void Oled::sendData(const uint8_t data) {
	device.start(IicBus::Write);
	device.transmit({0x40, data});
	device.stop();
}

void Oled::sendData(const uint8_t *data, const uint8_t size) {
	device.start(IicBus::Write);
	device.transmit({0x40});
	device.transmit(data, size);
	device.stop();
}

void Oled::setCursor(const uint8_t x, const uint8_t y) {
	uint8_t data[] = {
		(uint8_t)(0xB0 | y), //设置Y位置
		(uint8_t)(0x10 | ((x & 0xF0) >> 4)), //设置X位置高4位
		(uint8_t)(0x00 | (x & 0x0F)) //设置X位置低4位
	};
	sendCommand(data, sizeof(data));
}

Oled::Oled() {
}

Oled::Oled(IicBus* bus) {
	init(bus);
}

Oled::~Oled() {
}

void Oled::init(IicBus *bus) {
	device.init(bus, OLED_ADDRESS);
	memset(buffers, ' ', 4 * 16);

	//for (i = 0; i < 1000; i++) for (j = 0; j < 1000; j++);			//上电延时
	Os::delayMs(100); //延时100ms，等待OLED复位完成

	device.lock(-1);
	sendCommand(oledInitData, sizeof(oledInitData)); //初始化
	device.unlock();

	clear(); //先把缓冲0填满空格
	update(); //由于缓冲1的内容全是0，与空格不一样，因此这一次更新相当于把整个OLED都填满空格，不需要另写清屏代码了
}

void Oled::clear() {
	memset(buffers, ' ', 4 * 16); //将缓冲里面的内容全部用空格替代
}

void Oled::printChar(uint8_t x, uint8_t y, char c) {
	setCursor(x * 8, y * 2); //设置光标位置在上半部分
	sendData((uint8_t*)oledFont_8x16[c - ' '], 8); //显示上半部分内容

	setCursor(x * 8, y * 2 + 1); //设置光标位置在下半部分
	sendData((uint8_t*)&(oledFont_8x16[c - ' '][8]), 8); //显示下半部分内容
}

void Oled::show(uint8_t x, uint8_t y, const uint8_t *data, size_t size) {
	uint8_t begin = x;
	for(size_t i = 0 ; i < size; i++) {
		if(data[i] == '\n') {
			y++;
			x = begin;
			continue; //换下一行
		}

		if(data[i] >= ' ' && data[i] <= '~') {
			if(x >= 0 && x < 16 && y >= 0 && y < 4) buffers[0][y][x] = data[i]; //存到缓冲中
			x++; //下一个
		}
	}
}

void Oled::show(uint8_t x, uint8_t y, const Array<uint8_t> &arr) {
	show(x, y, arr.array(), arr.size());
}

void Oled::show(uint8_t x, uint8_t y, const Array<uint8_t> *arr) {
	show(x, y, arr->array(), arr->size());
}

void Oled::update() {
	if(device.lock(-1)) {
		for(uint8_t y = 0; y < 4; y++) {
			for(uint8_t x = 0; x < 16; x++) {
				if(buffers[0][y][x] != buffers[1][y][x]) { //此字符不同，更新一下
					printChar(x, y, buffers[0][y][x]);
				}
				buffers[1][y][x] = buffers[0][y][x]; //保存缓冲
			}
		}

		device.unlock();
	}

}