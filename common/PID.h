#ifndef MOTIONBOARDFIRMWARE_PID_H
#define MOTIONBOARDFIRMWARE_PID_H

class PID {
public:
    PID(float p, float i, float d, float bias, float frequency);

    PID();

    void reset();

    void set(float p, float i, float d, float bias, float frequency);

    void set(float p, float i, float d);

    float compute(float error);

    void setMaxIntegral(float maxIntegral);

    void setMaxOutput(float maxOutput);

private:
    float m_p, m_i, m_d, m_bias, m_frequency;
    float m_errorSum, m_lastError;
    float m_output;
    float m_maxIntegral;
    float m_maxOutput;
    bool m_initDone;
};


#endif //MOTIONBOARDFIRMWARE_PID_H
