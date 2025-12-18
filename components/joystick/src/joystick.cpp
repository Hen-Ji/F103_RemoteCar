#include "joystick.h"

Joystick::Joystick() {
}

Joystick::Joystick(Adc *adcx, uint8_t channelxId, uint16_t iox, Adc *adcy, uint8_t channelyId, uint16_t ioy) {
    init(adcx, channelxId, iox, adcy, channelyId, ioy);
}

Joystick::~Joystick() {
}

void Joystick::init(Adc *adcx, uint8_t channelxId, uint16_t iox, Adc *adcy, uint8_t channelyId, uint16_t ioy) {
    channelx.init(adcx, channelxId, iox);
    channely.init(adcy, channelyId, ioy);
}

JoystickStatus Joystick::get() {
    JoystickStatus status;
    status.x = channelx.get() / 4095.0 * 2 - 1;
    status.y = channely.get() / 4095.0 * 2 - 1;
    return status;
}
