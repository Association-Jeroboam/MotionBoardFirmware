#pragma once
#include <stdio.h>
#include "Logging.hpp"

#include "State.hpp"

namespace eHSM {
namespace Declare {
template <std::uint32_t MAX_EVENTS_HANDLED>
class GripBuoyState : public ::eHSM::State {
  public:
    GripBuoyState() : eHSM::State(&eventList_) {}

  private:
    void onEntry() {
        Logging::println("[ENTER] GripBuoyState\n");
    }

    void onExit() {
        Logging::println("[EXIT] GripBuoyState\n");
    }

    Declare::Array<Event, MAX_EVENTS_HANDLED> eventList_;
    E_DISABLE_COPY(GripBuoyState);
};
} // namespace Declare

} // namespace eHSM
