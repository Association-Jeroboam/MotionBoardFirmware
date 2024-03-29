#include "BuildConf.hpp"
#include "Logging.hpp"
#include "MotionBoard.hpp"
#include "Parameters.hpp"
#include "ch.hpp"
#include "hal.h"
#include <climits>
#include <cmath>

void Board::IO::initPWM() {
    palSetLineMode(MOTOR_LEFT_P_CHAN_LINE, MOTOR_LEFT_P_CHAN_LINE_MODE);
    palSetLineMode(MOTOR_RIGHT_P_CHAN_LINE, MOTOR_RIGHT_P_CHAN_LINE_MODE);
    palSetLineMode(MOTOR_LEFT_DIR_PIN, MOTOR_LEFT_DIR_PIN_MODE);
    palSetLineMode(MOTOR_RIGHT_DIR_PIN, MOTOR_RIGHT_DIR_PIN_MODE);
    pwmStart(&MOTOR_PWM_DRIVER, &pwmMotorConfig);
    setMotorDutyCycle(Peripherals::LEFT_MOTOR, 0.);
    setMotorDutyCycle(Peripherals::RIGHT_MOTOR, 0.);
}

void Board::IO::setMotorDutyCycle(Peripherals::Motor motor, float duty_cycle) {
    if (duty_cycle > DEFAULT_MAX_PID_OUTPUT || duty_cycle < -DEFAULT_MAX_PID_OUTPUT) {
        return;
    }

    uint16_t fraction = (uint16_t)(fabs(duty_cycle) * PWM_DUTY_CYCLE_PRECISION / DEFAULT_MAX_PID_OUTPUT);
    uint16_t channel;
    ioline_t dir_pin;
    if (motor == Peripherals::Motor::LEFT_MOTOR) {
        channel = MOTOR_LEFT_CHANNEL;
        dir_pin = MOTOR_LEFT_DIR_PIN;
    } else {
        channel = MOTOR_RIGHT_CHANNEL;
        dir_pin = MOTOR_RIGHT_DIR_PIN;
    }

    if (duty_cycle > 0) {
        if(channel == MOTOR_LEFT_CHANNEL){
            palWriteLine(dir_pin, MOTOR_LEFT_FORWARD);
        } else if (channel == MOTOR_RIGHT_CHANNEL) {
            palWriteLine(dir_pin, MOTOR_RIGHT_FORWARD);
        } else {
            return;
        }
    } else {
        if(channel == MOTOR_LEFT_CHANNEL){
            palWriteLine(dir_pin, MOTOR_LEFT_BACKWARD);
        } else if (channel == MOTOR_RIGHT_CHANNEL) {
            palWriteLine(dir_pin, MOTOR_RIGHT_BACKWARD);
        } else {
            return;
        }
    }

    pwmEnableChannel(&MOTOR_PWM_DRIVER,
                     channel,
                     PWM_FRACTION_TO_WIDTH(&MOTOR_PWM_DRIVER, PWM_DUTY_CYCLE_PRECISION, fraction));
}
