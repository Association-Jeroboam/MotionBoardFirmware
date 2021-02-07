#include "ch.hpp"
#include "hal.h"
#include "Board.hpp"
#include "BuildConf.hpp"
#include "Logging.hpp"
#include <climits>

#define PWM_COUNTING_FREQUENCY 20000000
#define PWM_OUTPUT_FREQUENCY   20000

#define DEADTIME_VALUE 40

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
    .bdtr = DEADTIME_VALUE,
    .dier = 0,
};

__extension__ QEIConfig leftEncoderConf {
    .mode        = QEI_MODE_QUADRATURE,
    .resolution  = QEI_BOTH_EDGES,
    .dirinv      = QEI_DIRINV_TRUE,
    .overflow    = QEI_OVERFLOW_WRAP,
    .min         = SHRT_MIN,
    .max         = SHRT_MIN,
    .notify_cb   = NULL,
    .overflow_cb = NULL,
};

__extension__ QEIConfig rightEncoderConf {
    .mode        = QEI_MODE_QUADRATURE,
    .resolution  = QEI_BOTH_EDGES,
    .dirinv      = QEI_DIRINV_FALSE,
    .overflow    = QEI_OVERFLOW_WRAP,
    .min         = SHRT_MIN,
    .max         = SHRT_MIN,
    .notify_cb   = NULL,
    .overflow_cb = NULL,
};

void Board::init() {
    Board::Com::initDrivers();
    Board::IO::initDrivers();
}

void Board::IO::initDrivers() {
    Logging::println("IO drivers init");

    //Motor PWM init
    palSetLineMode(MOTOR_LEFT_P_CHAN_LINE,  PAL_MODE_ALTERNATE(6));
    palSetLineMode(MOTOR_LEFT_N_CHAN_LINE,  PAL_MODE_ALTERNATE(6));
    palSetLineMode(MOTOR_RIGHT_P_CHAN_LINE, PAL_MODE_ALTERNATE(6));
    palSetLineMode(MOTOR_RIGHT_N_CHAN_LINE, PAL_MODE_ALTERNATE(6));
    pwmStart(&MOTOR_PWM_DRIVER, &pwmMotorConfig);
    setMotorDutyCycle(LEFT_MOTOR,  5000);
    setMotorDutyCycle(RIGHT_MOTOR, 5000);

    //Encoders init
    //Left encoder
    palSetLineMode(ENCODER_LEFT_CHAN1_LINE, PAL_MODE_ALTERNATE(ENCODER_LEFT_CHAN1_PIN_MODE));
    palSetLineMode(ENCODER_LEFT_CHAN2_LINE, PAL_MODE_ALTERNATE(ENCODER_LEFT_CHAN2_PIN_MODE));
    qeiStart(&LEFT_ENCODER_DRIVER, &leftEncoderConf);
    qeiEnable(&LEFT_ENCODER_DRIVER);

    //Right encoder
    palSetLineMode(ENCODER_RIGHT_CHAN1_LINE, PAL_MODE_ALTERNATE(ENCODER_RIGHT_CHAN1_PIN_MODE));
    palSetLineMode(ENCODER_RIGHT_CHAN2_LINE, PAL_MODE_ALTERNATE(ENCODER_RIGHT_CHAN2_PIN_MODE));
    qeiStart(&RIGHT_ENCODER_DRIVER, &rightEncoderConf);
    qeiEnable(&RIGHT_ENCODER_DRIVER);
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

int16_t Board::IO::getLeftEncoderCount(){
    int16_t encoderCount = qeiGetCount(&LEFT_ENCODER_DRIVER);
    qeiSetCount(&LEFT_ENCODER_DRIVER, 0);
    return encoderCount;
}

int16_t Board::IO::getRightEncoderCount(){
    int16_t encoderCount = qeiGetCount(&RIGHT_ENCODER_DRIVER);
    qeiSetCount(&RIGHT_ENCODER_DRIVER, 0);
    return encoderCount;
}

void Board::IO::toggleLED(){
    palToggleLine(LED_LINE);
}

void Board::Com::initDrivers() {
    Logging::println("Com drivers init");
}
