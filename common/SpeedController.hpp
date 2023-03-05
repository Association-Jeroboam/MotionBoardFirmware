/*
 * Stolen from https://github.com/EsialRobotik/asserv_nucleo/blob/master/src/SpeedController/
 * Thanks Jeff and Esial Robotics!
 */

#pragma once

#include <cstdint>

struct  SpeedControllerParameters {
    float outputValue;
    float speedGoal;
    float speedError;
};

class SpeedController
{
  public:
    explicit SpeedController(float speedKp, float speedKi, float outputLimit, float maxInputSpeed, float measureFrequency);
    virtual ~SpeedController(){};

    virtual float update(float actualSpeed);

    virtual void setGains(float Kp, float Ki);

    void setSpeedGoal(float speed);
    float getSpeedGoal() const
    {
        return m_params.speedGoal;
    }

    float getSpeedError() const
    {
        return m_params.speedError;
    }

    float getIntegratedOutput() const
    {
        return m_integratedOutput;
    }

    float getCurrentKp() const
    {
        return m_speedKp;
    }

    float getCurrentKi() const
    {
        return m_speedKi;
    }

    void resetIntegral()
    {
        //TODO: reset() et resetIntegral()
        m_params.speedError = 0;
        m_params.speedGoal  = 0;
        m_outputValue       = 0;
        m_integratedOutput  = 0;
    }

    void setMaxOutputLimit(float outputLimit)
    {
        m_outputLimit = outputLimit;
    }

    void setMaxInputLimit(float max)
    {
        m_inputLimit = max;
    }

    inline float getOutputValue() {return m_params.outputValue;};

    inline SpeedControllerParameters getParameters(){return m_params;};

  protected:
    float m_speedKp;
    float m_speedKi;

    static constexpr float MIN_INTEGRATION_SPEED_MM_PER_S = 10;
    SpeedControllerParameters m_params;
    float m_outputValue;
    float m_speedGoal;
    float m_integratedOutput;
    float m_speedError;

    float m_outputLimit;
    float m_inputLimit;

    float m_measureFrequency;
};
