#include "Motor.hpp"
#include "DataStreamer.hpp"
#include "LocalMath.hpp"
#include "Logging.hpp"
#include "MotionBoard.hpp"
#include "Parameters.hpp"
#include "Peripherals.hpp"
#include "Pll.h"

Motor::Motor(Peripherals::Encoder encoder,
             Peripherals::Motor   motor,
             float                wheelRadius) :
m_wheelRadius(wheelRadius),
m_speedController((float*)LEFT_MOTOR_KP,
                       (float*)LEFT_MOTOR_KI,
                       (float*)MOTOR_SPEED_RANGE,
                       DEFAULT_MAX_PID_OUTPUT,
                       MAX_WHEEL_SPEED, MOTOR_CONTROL_LOOP_FREQ),
m_encoder(encoder),
m_motor(motor),
m_pll(DEFAULT_PLL_BW)
{
    m_drivenDistance = 0.;
    m_disabled = false;
    m_tick_count = 0;
    m_tick_integral = 0;
}

void Motor::updateControl() {
    updateMeasure();
    float command;
    if(m_disabled) {
        command = 0.;
        m_speedController.resetIntegral();
    } else {
        command = m_speedController.update(m_speed);
    }
    Board::IO::setMotorDutyCycle(m_motor, command);
    if (m_encoder == Peripherals::LEFT_ENCODER) {
        DataStreamer::instance()->setEntry(leftPWMEnum, command);
    } else if (m_encoder == Peripherals::RIGHT_ENCODER) {
        DataStreamer::instance()->setEntry(rightPWMEnum, command);
    }
}

void Motor::updateMeasure() {
    int16_t encoderCount = Board::IO::getEncoderCount(m_encoder);
    m_tick_count += encoderCount;
    m_tick_integral += encoderCount;
    m_pll.update(encoderCount, MOTOR_CONTROL_LOOP_DT);
    m_speed = m_pll.getSpeed() * m_wheelRadius * (1. / ENCODER_TICK_PER_TURN) * GEAR_RATIO * 2. * M_PI;;
    float   drivenAngle  = float(encoderCount) * (1. / ENCODER_TICK_PER_TURN) * GEAR_RATIO * 2. * M_PI;
    m_drivenDistance += drivenAngle * m_wheelRadius;
}

void Motor::setPID(float p, float i, uint8_t range) {
    m_speedController.setGains(p, i, range);
}

void Motor::setSpeed(float speed) {
    if( speed > MAX_WHEEL_SPEED ) {
        speed = MAX_WHEEL_SPEED;
    } else if (speed < -MAX_WHEEL_SPEED) {
        speed = -MAX_WHEEL_SPEED;
    }
    m_speedController.setSpeedGoal(speed);
}

void Motor::setPIDThreshold(float threshold, uint8_t range) {
    m_speedController.setSpeedRange(threshold, range);
}

void Motor::setWheelRadius(float wheelRadius) {
    m_wheelRadius = wheelRadius;
}

float Motor::getSpeed() {
    return m_speed;
}

float Motor::getDrivenDistance() {
    float distance   = m_drivenDistance;
    m_drivenDistance = 0.;
    return distance;
}

void Motor::reset() {
    m_speedController.resetIntegral();
    m_speedController.setSpeedGoal(0.);
    Board::IO::setMotorDutyCycle(m_motor, 0);
}

int32_t Motor::getTickCount() {
    int32_t tick_count = m_tick_count;
    m_tick_count = 0;
    // Logging::print("%ld", m_tick_integral);
    // if(m_motor == Peripherals::RIGHT_MOTOR) {
    //     Logging::println("");
    // } else {
    //     Logging::print(" : ");
    // }
    return tick_count;
}

void Motor::setDisable(bool disable) {
    Board::IO::setBrake(m_motor, disable);

    if(m_disabled && !disable) {
        reset();
    }
    m_disabled = disable;
}
