#pragma once
#include "Logging.hpp"
#include <stdio.h>

#include "GoToBuoyState.hpp"
#include "GripBuoyState.hpp"
#include "State.hpp"
#include "Strategy/Events.hpp"
#include "TurnState.hpp"

namespace eHSM {
namespace Declare {
const uint32_t GO_TO_BUOY_STATE_EVENTS = 1;
const uint32_t GRIP_BUOY_STATE_EVENTS  = 1;
const uint32_t TURN_STATE_EVENTS       = 1;

template <std::uint8_t MAX_EVENTS_HANDLED>
class GetBuoyState : public ::eHSM::State {
  public:
    GetBuoyState() : eHSM::State(&eventList_) {
        turnState.setSuperstate(*this);
        goToBuoyState.setSuperstate(*this);
        gripBuoyState.setSuperstate(*this);

        this->setInitialSubState(turnState);

        turnState.addEvent(MoveOk, goToBuoyState);
        goToBuoyState.addEvent(MoveOk, gripBuoyState);
    }

    Declare::TurnState<TURN_STATE_EVENTS>           turnState;
    Declare::GoToBuoyState<GO_TO_BUOY_STATE_EVENTS> goToBuoyState;
    Declare::GripBuoyState<GRIP_BUOY_STATE_EVENTS>  gripBuoyState;

  private:
    void onEntry() {
        Logging::println("[ENTER] GetBuoyState\n");
    }

    void onExit() {
        Logging::println("[EXIT] GetBuoyState\n");
    }

    Declare::Array<Event, MAX_EVENTS_HANDLED> eventList_;
    E_DISABLE_COPY(GetBuoyState);
};
} // namespace Declare

} // namespace eHSM
