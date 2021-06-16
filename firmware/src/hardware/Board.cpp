#include "Board.hpp"
#include "BuildConf.hpp"
#include "Logging.hpp"
#include "Parameters.hpp"
#include "ch.hpp"
#include "hal.h"
#include <climits>

#define PWM_COUNTING_FREQUENCY 20000000
#define PWM_OUTPUT_FREQUENCY 20000

#define DEADTIME_VALUE 40

#define CONTROL_LOOP_TIMER_COUNTING_FREQUENCY 2000000

static void controlLoopTimerCallback(GPTDriver* gptp);
static void startMatchTimerCallback(GPTDriver* gptp);

static chibios_rt::EventSource eventSource;

__extension__ PWMChannelConfig channelConf{
    .mode     = PWM_OUTPUT_ACTIVE_LOW | PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW,
    .callback = NULL,
};

__extension__ const PWMConfig pwmMotorConfig{
    .frequency = PWM_COUNTING_FREQUENCY,
    .period    = PWM_COUNTING_FREQUENCY / PWM_OUTPUT_FREQUENCY,
    .callback  = NULL,
    .channels  = {
        channelConf,
        channelConf,
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
    },
    .cr2  = 0,
    .bdtr = DEADTIME_VALUE,
    .dier = 0,
};

__extension__ QEIConfig leftEncoderConf{
    .mode        = QEI_MODE_QUADRATURE,
    .resolution  = QEI_BOTH_EDGES,
    .dirinv      = QEI_DIRINV_TRUE,
    .overflow    = QEI_OVERFLOW_WRAP,
    .min         = SHRT_MIN,
    .max         = SHRT_MAX,
    .notify_cb   = NULL,
    .overflow_cb = NULL,
};

__extension__ QEIConfig rightEncoderConf{
    .mode        = QEI_MODE_QUADRATURE,
    .resolution  = QEI_BOTH_EDGES,
    .dirinv      = QEI_DIRINV_FALSE,
    .overflow    = QEI_OVERFLOW_WRAP,
    .min         = SHRT_MIN,
    .max         = SHRT_MAX,
    .notify_cb   = NULL,
    .overflow_cb = NULL,
};

__extension__ GPTConfig intervalTimerConfig{
    .frequency = CONTROL_LOOP_TIMER_COUNTING_FREQUENCY,
    .callback  = controlLoopTimerCallback,
    .cr2       = 0,
    .dier      = 0,
};

__extension__ GPTConfig startMatchTimerConfig{
        .frequency = 1000,
        .callback  = startMatchTimerCallback,
        .cr2       = 0,
        .dier      = 0,
};

void Board::init() {
    Board::Com::initDrivers();
    Board::IO::initDrivers();
}

void Board::IO::initDrivers() {
    Logging::println("IO drivers init");

    //Motor PWM init
    palSetLineMode(MOTOR_LEFT_P_CHAN_LINE, PAL_MODE_ALTERNATE(6));
    palSetLineMode(MOTOR_LEFT_N_CHAN_LINE, PAL_MODE_ALTERNATE(6));
    palSetLineMode(MOTOR_RIGHT_P_CHAN_LINE, PAL_MODE_ALTERNATE(6));
    palSetLineMode(MOTOR_RIGHT_N_CHAN_LINE, PAL_MODE_ALTERNATE(6));
    pwmStart(&MOTOR_PWM_DRIVER, &pwmMotorConfig);
    setMotorDutyCycle(Peripherals::LEFT_MOTOR, 0.);
    setMotorDutyCycle(Peripherals::RIGHT_MOTOR, 0.);

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

    //Event Timers
    gptStart(&MOTOR_CONTROL_LOOP_TIMER, &intervalTimerConfig);
    gptStart(&START_MATCH_TIMER, &startMatchTimerConfig);
}

void Board::IO::setMotorDutyCycle(Peripherals::Motor motor, float duty_cycle) {
    if (duty_cycle > DEFAULT_MAX_PID_OUTPUT || duty_cycle < -DEFAULT_MAX_PID_OUTPUT)
        return;
    uint16_t percentage = (uint16_t)((duty_cycle / (2 * DEFAULT_MAX_PID_OUTPUT) + 0.5) * PWM_MAX_DUTY_CYCLE_VALUE);
    pwmEnableChannel(&MOTOR_PWM_DRIVER,
                     motor,
                     PWM_FRACTION_TO_WIDTH(&MOTOR_PWM_DRIVER, PWM_MAX_DUTY_CYCLE_VALUE, percentage));
}

void Board::IO::deinitPWM() {
    Logging::println("PWM deinit");
    pwmDisableChannel(&MOTOR_PWM_DRIVER, Peripherals::LEFT_MOTOR);
    pwmDisableChannel(&MOTOR_PWM_DRIVER, Peripherals::RIGHT_MOTOR);
}

int16_t Board::IO::getEncoderCount(Peripherals::Encoder encoder) {
    int16_t encoderCount;

    switch (encoder) {
        case Peripherals::LEFT_ENCODER:
            encoderCount = qeiGetCount(&LEFT_ENCODER_DRIVER);
            qeiSetCount(&LEFT_ENCODER_DRIVER, 0);
            break;
        case Peripherals::RIGHT_ENCODER:
            encoderCount = qeiGetCount(&RIGHT_ENCODER_DRIVER);
            qeiSetCount(&RIGHT_ENCODER_DRIVER, 0);
            break;
    }
    return encoderCount;
}

void Board::IO::toggleLED() {
    palToggleLine(LED_LINE);
}

void Board::Com::initDrivers() {
    Logging::println("Com drivers init");
}

void Board::Events::startControlLoop(uint16_t frequency) {
    if (frequency > CONTROL_LOOP_TIMER_COUNTING_FREQUENCY) {
        frequency = CONTROL_LOOP_TIMER_COUNTING_FREQUENCY;
    }
    uint16_t interval = (uint16_t)(1.0 / (float)frequency * CONTROL_LOOP_TIMER_COUNTING_FREQUENCY);
    gptStartContinuous(&MOTOR_CONTROL_LOOP_TIMER, (gptcnt_t)interval);
}

void Board::Events::startStartMatchTimer(uint16_t interval_ms) {
    gptStartOneShot(&START_MATCH_TIMER, (gptcnt_t)interval_ms);
}

static void controlLoopTimerCallback(GPTDriver* gptp) {
    (void)gptp;
    eventSource.broadcastFlagsI(Board::Events::RUN_MOTOR_CONTROL);
}

static void startMatchTimerCallback(GPTDriver* gptp) {
    (void)gptp;
    eventSource.broadcastFlagsI(Board::Events::START_MATCH);
}

void Board::Events::eventRegister(chibios_rt::EventListener* elp, eventmask_t event) {
    eventSource.registerMask(elp, event);
}
