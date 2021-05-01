#pragma once
#include <stdio.h>

#include "../Events.hpp"
#include "GetBuoyState/GetBuoyState.hpp"
#include "LightHouseState/LightHouseState.hpp"
#include "State.hpp"

namespace eHSM {
namespace Declare {
const uint32_t GET_1_BUOY_STATE_EVENTS = 1;
const uint32_t GET_2_BUOY_STATE_EVENTS = 1;
const uint32_t LIGHTHOUSE_STATE_EVENTS = 1;

template <std::uint32_t MAX_EVENTS_HANDLED>
class MatchState : public ::eHSM::State {
  public:
    MatchState() : eHSM::State(&eventList_), getFirstBuoyState(1, 0), getSecondBuoyState(1, 1) {
        getFirstBuoyState.setSuperstate(*this);
        getSecondBuoyState.setSuperstate(*this);
        lightHouseState.setSuperstate(*this);

        this->setInitialSubState(getFirstBuoyState);

        getFirstBuoyState.gripBuoyState.addEvent(CanMove, getSecondBuoyState);
        getSecondBuoyState.gripBuoyState.addEvent(CanMove, lightHouseState);

        State::Action_t actionHolder;
        actionHolder.bind<Declare::LightHouseState, &Declare::LightHouseState::setCompassOk>(&lightHouseState);
        this->addEvent(CompassOk, actionHolder);
    }

    Declare::GetBuoyState<GET_1_BUOY_STATE_EVENTS> getFirstBuoyState;
    Declare::GetBuoyState<GET_2_BUOY_STATE_EVENTS> getSecondBuoyState;
    Declare::LightHouseState                       lightHouseState;

  private:
    void onEntry() {
        printf("[ENTER] MatchState\n");
    }

    void onExit() {
        printf("[EXIT] MatchState\n");
    }

    Declare::Array<Event, MAX_EVENTS_HANDLED> eventList_;
    E_DISABLE_COPY(MatchState);
};
} // namespace Declare

} // namespace eHSM
