#include "motor.h"

Motor::Motor() {
}

Motor::Motor(Timer *timerp, uint8_t channelpId, uint16_t iop, Timer *timern, uint8_t channelnId, uint16_t ion) {
    init(timerp, channelpId, iop, timern, channelnId, ion);
}

Motor::~Motor() {
}

void Motor::init(Timer *timerp, uint8_t channelpId, uint16_t iop, Timer *timern, uint8_t channelnId, uint16_t ion) {
    pwmp.init(timerp, iop, channelpId);
    pwmn.init(timern, ion, channelnId);
    pwmp.setDuty(0);
    pwmn.setDuty(0);
    timerp->enableOutput();
    timern->enableOutput();
}

void Motor::start() {
    pwmp.start();
    pwmn.start();
}

void Motor::stop() {
    pwmp.stop();
    pwmn.stop();
}

void Motor::setSpeed(float val) {
    if(val == 0) {
        pwmp.setDuty(0);
        pwmn.setDuty(0);
    }
    else if(val > 0) {
        pwmp.setDuty(val);
        pwmn.setDuty(0);
    }
    else if(val < 0) {
        pwmp.setDuty(0);
        pwmn.setDuty(-val);
    }
}
