#include "ch.hpp"
#include "hal.h"
#include "hal_pal.h"
#include "hal_serial.h"
#include "hal_pwm.h"
#include "Board.hpp"
#include "BuildConf.hpp"
#include "Logging.hpp"

#define PWM_COUNTING_FREQUENCY 2000000
#define PWM_OUTPUT_FREQUENCY   20000

#define MOTOR_LEFT_P_CHAN_LINE  LINE_ARD_D1
#define MOTOR_LEFT_N_CHAN_LINE  LINE_ARD_D3
#define MOTOR_RIGHT_P_CHAN_LINE LINE_ARD_D9
#define MOTOR_RIGHT_N_CHAN_LINE LINE_ARD_A6

__extension__ PWMChannelConfig channelConf {
        .mode = PWM_OUTPUT_ACTIVE_HIGH | PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH,
        .callback = NULL,
};

__extension__ const PWMConfig pwmMotorConfig {
    .frequency = PWM_COUNTING_FREQUENCY,
    .period = PWM_COUNTING_FREQUENCY/PWM_OUTPUT_FREQUENCY,
    .callback = NULL,
    .channels = {
            channelConf,
            channelConf,
            {PWM_OUTPUT_DISABLED, NULL},
            {PWM_OUTPUT_DISABLED, NULL},
    },
    .cr2  = 0,
    .bdtr = 0,
    .dier = 0,
};

void Board::IO::initDrivers() {
    Logging::println("IO drivers init");
    palSetLineMode(MOTOR_LEFT_P_CHAN_LINE,  PAL_MODE_ALTERNATE(6));
    palSetLineMode(MOTOR_LEFT_N_CHAN_LINE,  PAL_MODE_ALTERNATE(6));
    palSetLineMode(MOTOR_RIGHT_P_CHAN_LINE, PAL_MODE_ALTERNATE(6));
    palSetLineMode(MOTOR_RIGHT_N_CHAN_LINE, PAL_MODE_ALTERNATE(6));
    pwmStart(&MOTOR_PWM_DRIVER, &pwmMotorConfig);
    setMotorDutyCycle(LEFT_MOTOR,  5000);
    setMotorDutyCycle(RIGHT_MOTOR, 5000);

    Logging::println("pwm state %u", MOTOR_PWM_DRIVER.state);
}

void Board::IO::setMotorDutyCycle(enum motor motor, uint16_t duty_cycle){
    Logging::println("Setting motor %u duty cycle: %u", motor, duty_cycle);
    pwmEnableChannel(&MOTOR_PWM_DRIVER,
                     motor,
                     PWM_PERCENTAGE_TO_WIDTH(&MOTOR_PWM_DRIVER, duty_cycle));
}

void Board::IO::deinitPWM(){
    Logging::println("PWM deinit");
    pwmDisableChannel(&MOTOR_PWM_DRIVER, LEFT_MOTOR);
    pwmDisableChannel(&MOTOR_PWM_DRIVER, RIGHT_MOTOR);
}

void Board::IO::toggleLED(){
    palToggleLine(LED_LINE);
}

void Board::Com::initDrivers() {
    Logging::println("Com drivers init");
}
