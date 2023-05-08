#pragma once
#include "Control.hpp"
#include "ch.hpp"
#include "CanListener.hpp"

constexpr uint16_t CONTROL_THREAD_WA = 0x800;
constexpr uint8_t  CAN_PUBLISH_DIVIDER = 2;

enum ControlThreadFlags {
    GoalReached = 1 << 0,
};

class ControlThread : public chibios_rt::BaseStaticThread<CONTROL_THREAD_WA>,
                      public chibios_rt::EventSource,
                      public CanListener{
  public:
    static ControlThread* instance();
    void                  main() override;
    Control*              getControl();

    void processCanMsg(CanardRxTransfer * transfer);
    void setPublishTicksState(bool publish);

  private:
    ControlThread();
    static ControlThread s_instance;
    chibios_rt::EventListener m_boardListener;
    chibios_rt::EventListener m_avoidanceListener;

    void    processPoseMsg(CanardRxTransfer * transfer, float* x, float* y, float* theta);
    void    processTwistMsg(CanardRxTransfer * transfer, float* linear, float* angular);
    void    processAdaptativPIDMsg(CanardRxTransfer * transfer);
    void    processMotionConfigMsg(CanardRxTransfer * transfer);
    void    processSpeedCommandMsg(CanardRxTransfer * transfer);
    void    processPwmCommandMsg(CanardRxTransfer * transfer);
    void    sendOdomTicks();
    void    sendCurrentState();
    void    sendPIDStates();
    void    updateDataStreamer();
    bool    moveOkFired;
    bool    m_sendOdomTicks;
    Control control;
};
