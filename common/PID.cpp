#include "PID.h"


PID::PID(float p, float i, float d, float bias, float frequency) :
        m_p(p), m_i(i), m_d(d), m_bias(bias), m_frequency(frequency) {
    m_i /= m_frequency;
    m_d *= m_frequency;
    m_errorSum = 0.;
    m_lastError = 0.;
    m_output = 0.;
    m_initDone = false;
    m_maxIntegral = 1.;
}

PID::PID() : m_p(0.), m_i(0.), m_d(0.), m_bias(0.), m_frequency(1.) {
	m_lastError = 0.;
	m_output = 0.;
	m_initDone = false;
	m_maxIntegral = 1.;
}

void PID::reset() {
    m_initDone = false;
    m_lastError = 0.;
    m_errorSum = 0.;
}

void PID::set(float p, float i, float d, float bias, float frequency) {
    m_frequency = frequency;
    m_bias = bias;
    this->set(p, i, d);
}

void PID::set(float p, float i, float d) {
    m_p = p;
    m_i = i / m_frequency;
    m_d = d * m_frequency;
    this->reset();
}

float PID::compute(float error) {
    float error_D;

    if (!m_initDone) {
        //Do not use D in first run
        error_D = 0;
        m_initDone = true;
    } else {
        error_D = (error - m_lastError);
    }

    m_errorSum += (error + m_lastError) / 2.; // bilinear integration
    m_lastError = error;

    float P_part = m_p * error;
    float I_part = m_i * m_errorSum;
    float D_part = m_d * error_D;

    if (I_part > m_maxIntegral) {
        I_part = m_maxIntegral;
    } else if (I_part < -m_maxIntegral) {
        I_part = -m_maxIntegral;
    }

    m_output = P_part + I_part + D_part;

    //nothing happens if pid->m_output == 0
    if (m_output > 0) {
        m_output += m_bias;
    } else if (m_output < 0) {
        m_output -= m_bias;
    }
    return m_output;
}

void PID::setMaxIntegral(float maxIntegral) {
    m_maxIntegral = maxIntegral;
}