#pragma once
#include "Control.hpp"
#include "ch.hpp"

constexpr uint16_t CONTROL_WA = 0x200;

class ControlThread : public chibios_rt::BaseStaticThread<CONTROL_WA>,
                      public chibios_rt::EventListener {
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
