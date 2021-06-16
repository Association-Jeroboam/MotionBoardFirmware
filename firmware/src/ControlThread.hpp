#pragma once
#include "Control.hpp"
#include "ch.hpp"

constexpr uint16_t CONTROL_THREAD_WA = 0x200;

enum ControlThreadEvents {
    RunMotorControl = 1 << 0,
};

enum ControlThreadFlags {
    GoalReached = 1 << 0,
};

class ControlThread : public chibios_rt::BaseStaticThread<CONTROL_THREAD_WA>,
                      public chibios_rt::EventListener,
                      public chibios_rt::EventSource {
  public:
    static ControlThread* instance();
    void                  main() override;
    Control*              getControl();

  private:
    ControlThread();

    void    updateDataStreamer();
    bool    moveOkFired;
    Control control;
};
