#include "MotionBoard.hpp"
#include "BuildConf.hpp"
#include "CanRxThread.hpp"
#include "CanTxThread.hpp"
#include "Logging.hpp"
#include "Parameters.hpp"
#include "ch.hpp"
#include "hal.h"
#include <climits>
#include "canard.h"
#include "Memory.hpp"

#define CONTROL_LOOP_TIMER_COUNTING_FREQUENCY 2000000

static void controlLoopTimerCallback(GPTDriver* gptp);
static void gpioEmergencyStopCb(void* arg);

inline void* canardSpecificHeapAlloc(CanardInstance* ins, size_t amount) {
    (void)ins;
    return chHeapAlloc(NULL, amount);
}

inline void canardSpecificHeapFree(CanardInstance* ins, void* pointer) {
    (void)ins;
    if(pointer) chHeapFree(pointer);
}

static chibios_rt::EventSource eventSource;
chibios_rt::Timer matchTimer;
enum timerState {
    COMPASS,
    FLAG,
    END_MATCH,
};
timerState matchTimerState;

CanardInstance canardInstance;
CanTxThread canTxThread(&canardInstance);
CanRxThread canRxThread(&canardInstance);



__extension__ const GPTConfig intervalTimerConfig{
    .frequency = CONTROL_LOOP_TIMER_COUNTING_FREQUENCY,
    .callback  = controlLoopTimerCallback,
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
    Logging::println("init encoders");
    palSetLineMode(ENCODER_LEFT_CHAN1_LINE, ENCODER_LEFT_CHAN1_PIN_MODE);
    palSetLineMode(ENCODER_LEFT_CHAN2_LINE, ENCODER_LEFT_CHAN2_PIN_MODE);
    qeiStart(&LEFT_ENCODER_DRIVER, &leftEncoderConf);
    qeiEnable(&LEFT_ENCODER_DRIVER);

    //Right encoder
    palSetLineMode(ENCODER_RIGHT_CHAN1_LINE, ENCODER_RIGHT_CHAN1_PIN_MODE);
    palSetLineMode(ENCODER_RIGHT_CHAN2_LINE, ENCODER_RIGHT_CHAN2_PIN_MODE);
    qeiStart(&RIGHT_ENCODER_DRIVER, &rightEncoderConf);
    qeiEnable(&RIGHT_ENCODER_DRIVER);

    //Left Tachp
//    palSetLineMode(TACHO_LEFT_LINE, TACHO_LEFT_PIN_MODE);
//    icuStart(&TACHO_DRIVER_LEFT, &leftTachoConfig);
//    icuStartCapture(&TACHO_DRIVER_LEFT);
//
//    //Right Tachp
//    palSetLineMode(TACHO_RIGHT_LINE, TACHO_RIGHT_PIN_MODE);
//    icuStart(&TACHO_DRIVER_RIGHT, &rightTachoConfig);
//    icuStartCapture(&TACHO_DRIVER_RIGHT);
//    uint32_t width = icu_lld_get_width(&TACHO_DRIVER_RIGHT);
}

void Board::IO::initTimers() {
    gptStart(&MOTOR_CONTROL_LOOP_TIMER, &intervalTimerConfig);
}

void Board::IO::initGPIO() {
//    palSetLineMode(LED_LINE, LED_LINE_MODE);

    //TODO: This causes an assert in _pal_lld_enablepadevent, find out why
    palSetLineMode(EMGCY_STOP_PIN, EMGCY_STOP_PIN_MODE);
    palEnableLineEvent(EMGCY_STOP_PIN, PAL_EVENT_MODE_BOTH_EDGES);
    palSetLineCallback(EMGCY_STOP_PIN, gpioEmergencyStopCb, NULL);

    palSetLineMode(BRAKE_LEFT_PIN, BRAKE_LEFT_PIN_MODE);
    palSetLine(BRAKE_LEFT_PIN);
    palSetLineMode(BRAKE_RIGHT_PIN, BRAKE_LEFT_PIN_MODE);
    palSetLine(BRAKE_RIGHT_PIN);
}

void Board::IO::setBrake(Peripherals::Motor motor, bool brake){
    uint32_t palState = brake ? PAL_LOW : PAL_HIGH;

    if(motor == Peripherals::LEFT_MOTOR) {
        palWriteLine(BRAKE_LEFT_PIN, palState);
    } else {
        palWriteLine(BRAKE_RIGHT_PIN, palState);
    }
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
//            Logging::println("LEFT   %i", encoderCount);
            qeiSetCount(&LEFT_ENCODER_DRIVER, 0);
            break;
        case Peripherals::RIGHT_ENCODER:
            encoderCount = qeiGetCount(&RIGHT_ENCODER_DRIVER);
//            Logging::println("RIGHT %i", encoderCount);
            qeiSetCount(&RIGHT_ENCODER_DRIVER, 0);
            break;
    }
//    Logging::println("enc %i cnt: %i", encoder, encoderCount);
    return encoderCount;
}

float Board::IO::getMotorSpeed(Peripherals::Motor motor) {
//    uint32_t icuWidth = 0;
    uint32_t icuPeriod = 0;
    switch (motor) {
        case Peripherals::LEFT_MOTOR:
            icuPeriod = icu_lld_get_period(&TACHO_DRIVER_LEFT);
//            icuWidth = icu_lld_get_width(&TACHO_DRIVER_LEFT);
            break;
        case Peripherals::RIGHT_MOTOR:
            icuPeriod = icu_lld_get_period(&TACHO_DRIVER_RIGHT);
//            icuWidth = icu_lld_get_width(&TACHO_DRIVER_RIGHT);
            break;
    }
//    Logging::println("period %u width %u", icuPeriod, icuWidth);
    float motorSpeedRevPerSec = (float)icuPeriod / ICU_FREQUENCY;
    return 2 * M_PI * motorSpeedRevPerSec;
}

void Board::IO::toggleLED() {
//    palToggleLine(LED_LINE);
}

void Board::Com::initDrivers() {
    Logging::println("Com drivers init");
    CANBus::init();
}

void Board::Com::CANBus::init() {
    palSetLineMode(CAN_TX_PIN, CAN_TX_PIN_MODE);
    palSetLineMode(CAN_RX_PIN, CAN_RX_PIN_MODE);
    canStart(&CAN_DRIVER, &canConfig);
    canardInstance = canardInit(canardSpecificHeapAlloc, canardSpecificHeapFree);
    canardInstance.node_id = CAN_PROTOCOL_MOTION_BOARD_ID;
    canTxThread.start(NORMALPRIO);
    chThdSleep(TIME_MS2I(10));
    canRxThread.start(NORMALPRIO + 1);
    chThdSleep(TIME_MS2I(10));
    // let Threads finish initialization
    chThdYield();
}

bool Board::Com::CANBus::send(const CanardTransferMetadata* const metadata,
                              const size_t                        payload_size,
                              const void* const                   payload) {
    return canTxThread.send(metadata, payload_size, payload);
}

void Board::Com::CANBus::registerCanMsg(CanListener *listener,
                                          CanardTransferKind transfer_kind,
                                          CanardPortID port_id,
                                          size_t extent) {
    canRxThread.subscribe(listener, transfer_kind, port_id, extent);
}


void Board::Events::startControlLoop(uint16_t frequency) {
    if (frequency > CONTROL_LOOP_TIMER_COUNTING_FREQUENCY) {
        frequency = CONTROL_LOOP_TIMER_COUNTING_FREQUENCY;
    }
    uint16_t interval = (uint16_t)(1.0 / (float)frequency * CONTROL_LOOP_TIMER_COUNTING_FREQUENCY);
    gptStartContinuous(&MOTOR_CONTROL_LOOP_TIMER, (gptcnt_t)interval);
}

static void controlLoopTimerCallback(GPTDriver* gptp) {
    (void)gptp;
    eventSource.broadcastFlagsI(Board::Events::RUN_MOTOR_CONTROL);
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

