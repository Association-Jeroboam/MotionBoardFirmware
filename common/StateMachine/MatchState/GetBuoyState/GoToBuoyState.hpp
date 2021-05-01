#pragma once
#include <stdio.h>

#include "State.hpp"

namespace eHSM {
namespace Declare {
template <std::uint32_t MAX_EVENTS_HANDLED>
class GoToBuoyState : public ::eHSM::State {
  public:
    GoToBuoyState() : eHSM::State(&eventList_) {}

  private:
    void onEntry() {
        printf("[ENTER] GoToBuoyState\n");
    }

    void onExit() {
        printf("[EXIT] GoToBuoyState\n");
    }

    Declare::Array<Event, MAX_EVENTS_HANDLED> eventList_;
    E_DISABLE_COPY(GoToBuoyState);
};
} // namespace Declare

} // namespace eHSM
