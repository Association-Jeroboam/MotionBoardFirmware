/*
 * Stolen from https://github.com/EsialRobotik/asserv_nucleo/blob/master/src/SpeedController/
 * Thanks Jeff and Esial Robotics!
 */

#include "SpeedController.hpp"
#include <cmath>
#include <cstdlib>
#include <cstdint>

SpeedController::SpeedController(float speedKp, float speedKi, float outputLimit, float maxInputSpeed, float measureFrequency)
{
    m_params.outputValue = 0;
    m_params.speedGoal = 0;
    m_integratedOutput = 0;
    m_params.speedError = 0;
    m_speedKp = speedKp;
    m_speedKi = speedKi;

    m_outputLimit = outputLimit;
    m_inputLimit = maxInputSpeed;
    m_measureFrequency = measureFrequency;
}

float SpeedController::update(float actualSpeed)
{
    m_params.outputValue = 0;
    m_params.speedError = m_params.speedGoal - actualSpeed;

    // Regulateur en vitesse : un PI
    m_params.outputValue = m_params.speedError * m_speedKp;
    m_params.outputValue += m_integratedOutput;

    // On limite la sortie entre -m_outputLimit et m_outputLimit...
    bool limited = false;
    if (m_params.outputValue > m_outputLimit) {
        m_params.outputValue = m_outputLimit;
        limited = true;
    }
    if (m_params.outputValue < -m_outputLimit) {
        m_params.outputValue = -m_outputLimit;
        limited = true;
    }

    if (limited) // .. Si la sortie est limité, on désature l'intégrale
    {
        m_integratedOutput *= 0.9;
    }
    else    // .. Sinon, on integre l'erreur
    {
        m_integratedOutput += m_speedKi * m_params.speedError / m_measureFrequency;
        if (std::fabs(m_params.speedError) < MIN_INTEGRATION_SPEED_MM_PER_S) // Quand l'erreur de vitesse est proche de zero(ie: consigne à 0 et le robot ne bouge pas..), on désature l'intégrale
            m_integratedOutput *= 0.95;
    }

    // Protection antiWindup, le max est un peu empirique mais ça marche bien. Evite mieux les oscillations que m_outputLimit
    // float integrator_max_value = fabs(2 * m_params.speedError * m_speedKp);
    if (m_integratedOutput > m_outputLimit)
        m_integratedOutput = m_outputLimit;
    else if (m_integratedOutput < -m_outputLimit)
        m_integratedOutput = -m_outputLimit;

    return m_params.outputValue;
}

void SpeedController::setSpeedGoal(float speed)
{
    if (speed > m_inputLimit)
        speed = m_inputLimit;
    if (speed < -m_inputLimit)
        speed = -m_inputLimit;

    if (speed == 0.0)
        resetIntegral();
    m_params.speedGoal = speed;
}
;

void SpeedController::setGains(float Kp, float Ki)
{
    m_speedKp = Kp;
    m_speedKi = Ki;
    resetIntegral();
}
