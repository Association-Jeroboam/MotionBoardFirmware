#pragma once
#include "Control.hpp"
#include "ch.hpp"
#include "CanListener.hpp"

constexpr uint16_t CONTROL_THREAD_WA = 0x800;

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

  private:
    ControlThread();
    static ControlThread s_instance;
    chibios_rt::EventListener m_boardListener;
    chibios_rt::EventListener m_avoidanceListener;

    void    processPoseGoal(CanardRxTransfer * transfer);
    void    processTwistGoal(CanardRxTransfer * transfer);
    void    sendCurrentState();
    void    updateDataStreamer();
    bool    moveOkFired;
    Control control;
};
