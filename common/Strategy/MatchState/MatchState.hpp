#pragma once
#include "Logging.hpp"
#include <stdio.h>

#include "../Events.hpp"
#include "GetBuoyState/GetBuoyState.hpp"
#include "LightHouseState/LightHouseState.hpp"
#include "State.hpp"
#include "ThinkingState.hpp"

namespace eHSM {
namespace Declare {
const uint8_t THIKING_STATE_EVENTS    = 1;
const uint8_t GET_BUOY_STATE_EVENTS   = 1;
const uint8_t LIGHTHOUSE_STATE_EVENTS = 1;

template <std::uint8_t MAX_EVENTS_HANDLED>
class MatchState : public ::eHSM::State {
  public:
    MatchState() : eHSM::State(&eventList_) {
        thinkingState.setSuperstate(*this);
        getBuoyState.setSuperstate(*this);
        lightHouseState.setSuperstate(*this);

        this->setInitialSubState(thinkingState);

        thinkingState.addEvent(DoGetBuoy, getBuoyState);
        getBuoyState.gripBuoyState.addEvent(CanMove, thinkingState);

        State::Action_t actionHolder;
        actionHolder.bind<Declare::LightHouseState, &Declare::LightHouseState::setCompassOk>(&lightHouseState);
        this->addEvent(CompassOk, actionHolder);
    }

    Declare::ThinkingState<THIKING_STATE_EVENTS> thinkingState;
    Declare::GetBuoyState<GET_BUOY_STATE_EVENTS> getBuoyState;
    Declare::LightHouseState                     lightHouseState;

  private:
    void onEntry() {
        Logging::println("[ENTER] MatchState");
    }

    void onExit() {
        Logging::println("[EXIT] MatchState");
    }

    Declare::Array<Event, MAX_EVENTS_HANDLED> eventList_;
    E_DISABLE_COPY(MatchState);
};
} // namespace Declare

} // namespace eHSM
