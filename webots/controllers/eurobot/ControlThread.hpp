#pragma once

#include "Control.hpp"
#include "Parameters.hpp"

class ControlThread {
  public:
    static ControlThread* instance();
    void                  main();
    Control*              getControl();

  private:
    static const unsigned int controlSteps = MOTOR_CONTROL_LOOP_DT * 1000 / 8;
    unsigned int              controlCounter;
    Control                   control;
};
