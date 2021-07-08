#include "MotionBoard.hpp"
#include "BuildConf.hpp"
#include "CanRxThread.hpp"
#include "CanTxThread.hpp"
#include "Logging.hpp"
#include "Parameters.hpp"
#include "ch.hpp"
#include "hal.h"
#include <climits>

#define CONTROL_LOOP_TIMER_COUNTING_FREQUENCY 2000000

static void controlLoopTimerCallback(GPTDriver* gptp);
static void startMatchTimerCallback(GPTDriver* gptp);
static void gpioStartMatchCb(void* arg);
static void gpioEmergencyStopCb(void* arg);

static chibios_rt::EventSource eventSource;

CanTxThread canTxThread;
CanRxThread canRxThread;

__extension__ const QEIConfig leftEncoderConf{
    .mode        = QEI_MODE_QUADRATURE,
    .resolution  = QEI_BOTH_EDGES,
    .dirinv      = QEI_DIRINV_TRUE,
    .overflow    = QEI_OVERFLOW_WRAP,
    .min         = 0,
    .max         = SHRT_MAX,
    .notify_cb   = NULL,
    .overflow_cb = NULL,
};

__extension__ const QEIConfig rightEncoderConf{
    .mode        = QEI_MODE_QUADRATURE,
    .resolution  = QEI_BOTH_EDGES,
    .dirinv      = QEI_DIRINV_FALSE,
    .overflow    = QEI_OVERFLOW_WRAP,
    .min         = 0,
    .max         = SHRT_MAX,
    .notify_cb   = NULL,
    .overflow_cb = NULL,
};

__extension__ const GPTConfig intervalTimerConfig{
    .frequency = CONTROL_LOOP_TIMER_COUNTING_FREQUENCY,
    .callback  = controlLoopTimerCallback,
    .cr2       = 0,
    .dier      = 0,
};

__extension__ const GPTConfig startMatchTimerConfig{
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
    initPWM();
    initEncoders();
    initTimers();
    initGPIO();
}

void Board::IO::initEncoders() {
    //Left encoder
    palSetLineMode(ENCODER_LEFT_CHAN1_LINE, ENCODER_LEFT_CHAN1_PIN_MODE);
    palSetLineMode(ENCODER_LEFT_CHAN2_LINE, ENCODER_LEFT_CHAN2_PIN_MODE);
    qeiStart(&LEFT_ENCODER_DRIVER, &leftEncoderConf);
    qeiEnable(&LEFT_ENCODER_DRIVER);

    //Right encoder
    palSetLineMode(ENCODER_RIGHT_CHAN1_LINE, ENCODER_RIGHT_CHAN1_PIN_MODE);
    palSetLineMode(ENCODER_RIGHT_CHAN2_LINE, ENCODER_RIGHT_CHAN2_PIN_MODE);
    qeiStart(&RIGHT_ENCODER_DRIVER, &rightEncoderConf);
    qeiEnable(&RIGHT_ENCODER_DRIVER);
}

void Board::IO::initTimers() {
    gptStart(&MOTOR_CONTROL_LOOP_TIMER, &intervalTimerConfig);
    //    gptStart(&START_MATCH_TIMER, &startMatchTimerConfig);
}

void Board::IO::initGPIO() {
    palSetLineMode(LED_LINE, LED_LINE_MODE);

    palSetLineMode(START_PIN, START_PIN_MODE);
    palEnableLineEvent(START_PIN, PAL_EVENT_MODE_RISING_EDGE);
    palSetLineCallback(START_PIN, gpioStartMatchCb, NULL);

    palSetLineMode(STRATEGY_1_PIN, STRATEGY_1_PIN_MODE);

    palSetLineMode(STRATEGY_2_PIN, STRATEGY_2_PIN_MODE);

    palSetLineMode(SIDE_PIN, SIDE_PIN_MODE);

    palSetLineMode(EMGCY_STOP_PIN, EMGCY_STOP_PIN_MODE);
    palEnableLineEvent(EMGCY_STOP_PIN, PAL_EVENT_MODE_BOTH_EDGES);
    palSetLineCallback(EMGCY_STOP_PIN, gpioEmergencyStopCb, NULL);

    palSetLineMode(BRAKE_PIN, BRAKE_PIN_MODE);
    palSetLine(BRAKE_PIN);
}

bool Board::IO::getSide() {
    return (palReadLine(SIDE_PIN) == PAL_HIGH);
}

bool Board::IO::getStart() {
    return (palReadLine(START_PIN) == PAL_HIGH);
}

void Board::IO::setBrake(Peripherals::Motor motor, bool brake){
    static bool leftBrake = false;
    static bool rightBrake = false;
    if(motor == Peripherals::LEFT_MOTOR) {
        leftBrake = brake;
    } else {
        rightBrake = brake;
    }


    palWriteLine(BRAKE_PIN, (leftBrake || rightBrake) ? PAL_LOW : PAL_HIGH);
}

uint8_t Board::IO::getStrategy() {
    uint8_t strategy = 0;
    strategy |= palReadLine(STRATEGY_1_PIN) << 0;
    strategy |= palReadLine(STRATEGY_2_PIN) << 1;
    return strategy;
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
    CANBus::init();
    Lidar::init();
}

void Board::Com::CANBus::init() {
    palSetLineMode(CAN_TX_PIN, CAN_TX_PIN_MODE);
    palSetLineMode(CAN_RX_PIN, CAN_RX_PIN_MODE);
    canStart(&CAN_DRIVER, &canConfig);
    canTxThread.start(NORMALPRIO);
    canRxThread.start(NORMALPRIO + 1);
    // let Threads finish initialization
    chThdYield();
}

bool Board::Com::CANBus::send(canFrame_t canData) {
    return canTxThread.send(canData);
}

void Board::Com::CANBus::registerListener(CanListener* listener) {
    canRxThread.registerListener(listener);
}

void Board::Com::Lidar::init() {
    palSetLineMode(LIDAR_SD_TX_PIN, LIDAR_SD_TX_PIN_MODE);
    palSetLineMode(LIDAR_SD_RX_PIN, LIDAR_SD_RX_PIN_MODE);
    sdStart(&LIDAR_SD_DRIVER, &lidarSDConfig);
}

void Board::Events::startControlLoop(uint16_t frequency) {
    if (frequency > CONTROL_LOOP_TIMER_COUNTING_FREQUENCY) {
        frequency = CONTROL_LOOP_TIMER_COUNTING_FREQUENCY;
    }
    uint16_t interval = (uint16_t)(1.0 / (float)frequency * CONTROL_LOOP_TIMER_COUNTING_FREQUENCY);
    gptStartContinuous(&MOTOR_CONTROL_LOOP_TIMER, (gptcnt_t)interval);
}

void Board::Events::startStartMatchTimer(uint16_t interval_ms) {
    //    gptStartOneShot(&START_MATCH_TIMER, (gptcnt_t)interval_ms);
}

static void controlLoopTimerCallback(GPTDriver* gptp) {
    (void)gptp;
    eventSource.broadcastFlagsI(Board::Events::RUN_MOTOR_CONTROL);
}

static void startMatchTimerCallback(GPTDriver* gptp) {
    (void)gptp;
    eventSource.broadcastFlagsI(Board::Events::START_MATCH);
}

static void gpioStartMatchCb(void* arg) {
    palDisableLineEvent(START_PIN);
    eventSource.broadcastFlagsI(Board::Events::START_MATCH);
}

static void gpioEmergencyStopCb(void* arg) {
    if (palReadLine(EMGCY_STOP_PIN) == PAL_LOW) {
        eventSource.broadcastFlagsI(Board::Events::EMERGENCY_STOP);
    } else {
        eventSource.broadcastFlagsI(Board::Events::EMERGENCY_CLEARED);
    }
}

void Board::Events::eventRegister(chibios_rt::EventListener* elp, eventmask_t event) {
    eventSource.registerMask(elp, event);
}
