#pragma once

#include "Event.hpp"
#include "actions/Action.hpp"

enum StrategyState {
    IDLE,
    FIRST_ACTIONS,
    LAST_ACTIONS,
    MATCH_FINISHED
};

class Strategy : Action<StrategyState> {
  public:
    Strategy();

    void update();
};
