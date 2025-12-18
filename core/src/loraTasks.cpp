#include "loraTasks.h"
#include "lora.h"
#include "spiHardware.h"
#include "cstring.h"
#include "format.h"
#include "pid.h"
#include "math.h"
#include "vector.h"

void loraReceiveTask(void *args) {
    OsGroupMember * loraMember = new OsGroupMember(loraGroup, "lora"); //加入群组
    SpiBusHardware* spiBus = new SpiBusHardware(PA6, PA7, PA5, 1, 9000000); //创建SPI总线
    Lora* lora = new Lora(spiBus, PA4, PA8, PA10, PA9); //创建Lora模块
    lora->setBandwidth(Lora::Bandwidth500KHz); //设置500KHz带宽
    lora->setReceiveMode(); //设置为接收模式
    String* buffer = new String(128);

    while(1) {
        if(lora->receive(buffer, -1)) { //接收数据
            char* str = (char*)buffer->array(); //获取字符串
            str[buffer->size()] = 0; //添加结束符
            loraMember->send(str, buffer->size() + 1); //发送到群组
        }
        Os::delay(10);
    }
}
