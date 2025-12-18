#include "joystickTask.h"
#include "joystick.h"
#include "spiHardware.h"
#include "lora.h"
#include "cstring.h"
#include "math.h"

void joystickTask(void* args) {
    Adc* adc = new Adc(1);
    Joystick* stick1 = new Joystick(adc, 1, PA1, adc, 0, PA0); //创建摇杆
    Joystick* stick2 = new Joystick(adc, 8, PB0, adc, 9, PB1);
    SpiBusHardware* spiBus = new SpiBusHardware(PA6, PA7, PA5, 1, 9000000); //创建SPI总线
    Lora* lora = new Lora(spiBus, PA4, PA8, PA10, PA9); //创建Lora
    lora->setBandwidth(Lora::Bandwidth500KHz); //500KHz带宽
    lora->setTransmitMode(); //设置为发送模式

    String* buffer = new String(128);

    while(1) {
        JoystickStatus status1 = stick1->get(); //获取摇杆状态
        JoystickStatus status2 = stick2->get();

        lora->transmit(buffer->format("motor %.2f %.2f %.2f %.2f", status1.x, status1.y, status2.x, status2.y)); //发送给小车
        Os::delay(20);
    }
}