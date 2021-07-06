#pragma once
#include "Logging.hpp"
#include <stdio.h>

#include "State.hpp"

namespace eHSM {
namespace Declare {
template <std::uint8_t MAX_EVENTS_HANDLED>
class ThinkingState : public ::eHSM::State {
  public:
    ThinkingState() : eHSM::State(&eventList_) {}

  private:
    void onEntry() {
        Logging::println("[ENTER] ThinkingState\n");
    }

    void onExit() {
        Logging::println("[EXIT] ThinkingState\n");
    }

    Declare::Array<Event, MAX_EVENTS_HANDLED> eventList_;
    E_DISABLE_COPY(ThinkingState);
};
} // namespace Declare

} // namespace eHSM
