#pragma once
#include <stdio.h>

#include "../../Strategy.hpp"
#include "State.hpp"

namespace eHSM {
namespace Declare {
template <std::uint32_t MAX_EVENTS_HANDLED>
class GoToBuoyState : public ::eHSM::State {
  public:
    GoToBuoyState(float x, float y) : eHSM::State(&eventList_), x(x), y(y) {}

  private:
    void onEntry() {
        printf("[ENTER] GoToBuoyState\n");
        Goal goal(x, y, Goal::COORD);
        Strategy::instance()->control->setCurrentGoal(goal);
    }

    void onExit() {
        printf("[EXIT] GoToBuoyState\n");
    }

    float                                     x;
    float                                     y;
    Declare::Array<Event, MAX_EVENTS_HANDLED> eventList_;
    E_DISABLE_COPY(GoToBuoyState);
};
} // namespace Declare

} // namespace eHSM
