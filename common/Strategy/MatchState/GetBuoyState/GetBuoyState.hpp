#pragma once
#include <stdio.h>

#include "GoToBuoyState.hpp"
#include "GripBuoyState.hpp"
#include "State.hpp"

namespace eHSM {
namespace Declare {
const uint32_t GO_TO_BUOY_STATE_EVENTS = 1;
const uint32_t GRIP_BUOY_STATE_EVENTS  = 1;

template <std::uint32_t MAX_EVENTS_HANDLED>
class GetBuoyState : public ::eHSM::State {
  public:
    GetBuoyState(float x, float y) : eHSM::State(&eventList_), goToBuoyState(x, y) {
        goToBuoyState.setSuperstate(*this);
        gripBuoyState.setSuperstate(*this);

        this->setInitialSubState(goToBuoyState);

        goToBuoyState.addEvent(MoveOk, gripBuoyState);
    }

    Declare::GoToBuoyState<GO_TO_BUOY_STATE_EVENTS> goToBuoyState;
    Declare::GripBuoyState<GRIP_BUOY_STATE_EVENTS>  gripBuoyState;

  private:
    void onEntry() {
        printf("[ENTER] GetBuoyState\n");
    }

    void onExit() {
        printf("[EXIT] GetBuoyState\n");
    }

    Declare::Array<Event, MAX_EVENTS_HANDLED> eventList_;
    E_DISABLE_COPY(GetBuoyState);
};
} // namespace Declare

} // namespace eHSM
