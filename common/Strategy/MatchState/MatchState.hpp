#pragma once
#include <stdio.h>
#include "Logging.hpp"

#include "../Events.hpp"
#include "GetBuoyState/GetBuoyState.hpp"
#include "LightHouseState/LightHouseState.hpp"
#include "State.hpp"

namespace eHSM {
namespace Declare {
const uint32_t GET_1_BUOY_STATE_EVENTS = 1;
const uint32_t GET_2_BUOY_STATE_EVENTS = 1;
const uint32_t GET_3_BUOY_STATE_EVENTS = 1;
const uint32_t GET_4_BUOY_STATE_EVENTS = 1;
const uint32_t LIGHTHOUSE_STATE_EVENTS = 1;

template <std::uint32_t MAX_EVENTS_HANDLED>
class MatchState : public ::eHSM::State {
  public:
    MatchState() : eHSM::State(&eventList_), getFirstBuoyState(500, 0),
                                             getSecondBuoyState(500, 500),
                                             getThirdBuoyState(0, 500),
                                             getFourthBuoyState(0, 0){
        getFirstBuoyState.setSuperstate(*this);
        getSecondBuoyState.setSuperstate(*this);
        getThirdBuoyState.setSuperstate(*this);
        getFourthBuoyState.setSuperstate(*this);
        lightHouseState.setSuperstate(*this);

        this->setInitialSubState(getFirstBuoyState);

        getFirstBuoyState.gripBuoyState.addEvent(CanMove, getSecondBuoyState);
        getSecondBuoyState.gripBuoyState.addEvent(CanMove, getThirdBuoyState);
        getThirdBuoyState.gripBuoyState.addEvent(CanMove, getFourthBuoyState);
        getFourthBuoyState.gripBuoyState.addEvent(CanMove, getFirstBuoyState);


        State::Action_t actionHolder;
        actionHolder.bind<Declare::LightHouseState, &Declare::LightHouseState::setCompassOk>(&lightHouseState);
        this->addEvent(CompassOk, actionHolder);
    }

    Declare::GetBuoyState<GET_1_BUOY_STATE_EVENTS> getFirstBuoyState;
    Declare::GetBuoyState<GET_2_BUOY_STATE_EVENTS> getSecondBuoyState;
    Declare::GetBuoyState<GET_3_BUOY_STATE_EVENTS> getThirdBuoyState;
    Declare::GetBuoyState<GET_4_BUOY_STATE_EVENTS> getFourthBuoyState;
    Declare::LightHouseState                       lightHouseState;

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
