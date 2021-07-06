#pragma once
#include <stdio.h>

#include "State.hpp"

namespace eHSM {
namespace Declare {
template <std::uint8_t MAX_EVENTS_HANDLED>
class GoToBuoyState : public ::eHSM::State {
  public:
    GoToBuoyState();

  private:
    void                                      onEntry();
    void                                      onExit();
    Declare::Array<Event, MAX_EVENTS_HANDLED> eventList_;
    E_DISABLE_COPY(GoToBuoyState);
};
} // namespace Declare

} // namespace eHSM
