#pragma once
#include "HierarchicalStateMachine.hpp"
#include "IdleState.hpp"
#include "MatchState/MatchState.hpp"

using namespace eHSM;

const uint32_t IDLE_STATE_EVENTS  = 1;
const uint32_t MATCH_STATE_EVENTS = 3;

class StateMachine : public HierarchicalStateMachine {
  public:
    StateMachine() {
        idleState.addEvent(StartMatch, matchState);

        matchState.addEvent(EmergencyStop, idleState);
        matchState.addEvent(EndMatch, idleState);

        setInitialState(idleState);
    };

  protected:
    Declare::IdleState<IDLE_STATE_EVENTS>   idleState;
    Declare::MatchState<MATCH_STATE_EVENTS> matchState;
};
