#pragma once
#include "Control.hpp"
#include "HierarchicalStateMachine.hpp"
#include "IdleState.hpp"
#include "MatchState/MatchState.hpp"

using namespace eHSM;

const uint32_t IDLE_STATE_EVENTS  = 1;
const uint32_t MATCH_STATE_EVENTS = 3;

class Strategy : public HierarchicalStateMachine {

  private:
    Strategy() {
        idleState.addEvent(StartMatch, matchState);

        matchState.addEvent(EmergencyStop, idleState);
        matchState.addEvent(EndMatch, idleState);

        setInitialState(idleState);
    };

  protected:
    Declare::IdleState<IDLE_STATE_EVENTS>   idleState;
    Declare::MatchState<MATCH_STATE_EVENTS> matchState;

  public:
    static Strategy* instance();

    void setControl(Control* control_) {
        control = control_;
    }

    Control* control;
};
