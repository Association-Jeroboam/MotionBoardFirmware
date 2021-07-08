#pragma once
#include "Control.hpp"
#include "ch.hpp"

constexpr uint16_t CONTROL_THREAD_WA = 0x800;


enum ControlThreadFlags {
    GoalReached = 1 << 0,
};

class ControlThread : public chibios_rt::BaseStaticThread<CONTROL_THREAD_WA>,
                      public chibios_rt::EventSource {
  public:
    static ControlThread* instance();
    void                  main() override;
    Control*              getControl();

  private:
    ControlThread();
    static ControlThread s_instance;
    chibios_rt::EventListener m_boardListener;
    chibios_rt::EventListener m_avoidanceListener;

    void    updateDataStreamer();
    bool    moveOkFired;
    Control control;
};
