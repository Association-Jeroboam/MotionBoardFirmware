#include "Board.hpp"
#include "BuildConf.hpp"
#include "Logging.hpp"
#include "Parameters.hpp"
#include "ch.hpp"
#include "hal.h"
#include <climits>

void Board::IO::initPWM() {
    palSetLineMode(MOTOR_LEFT_P_CHAN_LINE,  MOTOR_LEFT_P_CHAN_LINE_MODE);
    palSetLineMode(MOTOR_LEFT_N_CHAN_LINE,  MOTOR_LEFT_N_CHAN_LINE_MODE);
    palSetLineMode(MOTOR_RIGHT_P_CHAN_LINE, MOTOR_RIGHT_P_CHAN_LINE_MODE);
    palSetLineMode(MOTOR_RIGHT_N_CHAN_LINE, MOTOR_RIGHT_N_CHAN_LINE_MODE);
    pwmStart(&MOTOR_PWM_DRIVER, &pwmMotorConfig);
    setMotorDutyCycle(Peripherals::LEFT_MOTOR, 0.);
    setMotorDutyCycle(Peripherals::RIGHT_MOTOR, 0.);
}

void Board::IO::setMotorDutyCycle(Peripherals::Motor motor, float duty_cycle) {
    if (duty_cycle > DEFAULT_MAX_PID_OUTPUT || duty_cycle < -DEFAULT_MAX_PID_OUTPUT)
        return;
    uint16_t percentage = (uint16_t)((duty_cycle / (2 * DEFAULT_MAX_PID_OUTPUT) + 0.5) * PWM_MAX_DUTY_CYCLE_VALUE);
    uint16_t channel;
    if(motor == Peripherals::Motor::LEFT_MOTOR) {
        channel = MOTOR_LEFT_CHANNEL;
    } else {
        channel = MOTOR_RIGHT_CHANNEL;
    }
    pwmEnableChannel(&MOTOR_PWM_DRIVER,
                     channel,
                     PWM_FRACTION_TO_WIDTH(&MOTOR_PWM_DRIVER, PWM_MAX_DUTY_CYCLE_VALUE, percentage));
}
