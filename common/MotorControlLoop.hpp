#ifndef MOTIONBOARDFIRMWARE_MOTORCONTROLLOOP_HPP
#define MOTIONBOARDFIRMWARE_MOTORCONTROLLOOP_HPP

#include "ch.hpp"

constexpr uint16_t MOTOR_CONTROL_LOOP_WA = 0x100;

class MotorControlLoop: public chibios_rt::BaseStaticThread<MOTOR_CONTROL_LOOP_WA>,
                        public chibios_rt::EventListener {
public:
    MotorControlLoop();

    void main() override;

};


#endif //MOTIONBOARDFIRMWARE_MOTORCONTROLLOOP_HPP
