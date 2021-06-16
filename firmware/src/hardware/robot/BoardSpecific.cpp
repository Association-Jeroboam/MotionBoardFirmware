#include "Board.hpp"
#include "BuildConf.hpp"
#include "Logging.hpp"
#include "Parameters.hpp"
#include "ch.hpp"
#include "hal.h"
#include <climits>

void Board::IO::initPWM() {
    palSetLineMode(MOTOR_LEFT_P_CHAN_LINE, MOTOR_LEFT_P_CHAN_LINE_MODE);
    palSetLineMode(MOTOR_RIGHT_P_CHAN_LINE, MOTOR_RIGHT_P_CHAN_LINE_MODE);
    pwmStart(&MOTOR_PWM_DRIVER, &pwmMotorConfig);
    setMotorDutyCycle(Peripherals::LEFT_MOTOR, 0.);
    setMotorDutyCycle(Peripherals::RIGHT_MOTOR, 0.);
}