#pragma once
#include <stdio.h>

#include "State.hpp"

namespace eHSM {
namespace Declare {
template <std::uint32_t MAX_EVENTS_HANDLED>
class IdleState : public ::eHSM::State {
  public:
    IdleState() : eHSM::State(&eventList_) {}

  private:
    void onEntry() {
        printf("[ENTER] IdleState\n");
    }

    void onExit() {
        printf("[EXIT] IdleState\n");
    }

    Declare::Array<Event, MAX_EVENTS_HANDLED> eventList_;
    E_DISABLE_COPY(IdleState);
};
} // namespace Declare

} // namespace eHSM
