#pragma once

#include <array>
#include <cstring>
#include <utility>

#include "Control.hpp"
#include "Events.hpp"
#include "HierarchicalStateMachine.hpp"
#include "IdleState.hpp"
#include "MatchState/MatchState.hpp"
#include "Switchers.hpp"

struct GetBuoyData {
    int16_t x;
    int16_t y;
};

using TestData = float;

typedef union {
    GetBuoyData buoyData;
    TestData    testData;
} EventData;

struct Action {
    Event     event;
    EventData eventData;
    bool      end = false;
};

using StrategyType = const std::array<Action, 4>;

StrategyType matchStrategy = {
    (Action){
        .event              = Event::DoGetBuoy,
        .eventData.buoyData = {
            .x = 500,
            .y = 0},
    },
    (Action){
        .event              = Event::DoGetBuoy,
        .eventData.buoyData = {.x = 500, .y = 500},
    },
    (Action){
        .event              = Event::DoGetBuoy,
        .eventData.buoyData = {.x = 0, .y = 500},
    },
    (Action){
        .event              = Event::DoGetBuoy,
        .eventData.buoyData = {.x = 0, .y = 0},
    },
};

StrategyType approvalStrategy = {
    (Action){
        .event              = Event::DoGetBuoy,
        .eventData.buoyData = {
            .x = 666,
            .y = 666},
    },
    (Action){
        .event              = Event::DoLightHouse,
        .eventData.testData = 3.14,
    },
    (Action){
        .end = true,
    },
};

const std::array<StrategyType*, 2> strategies = {&matchStrategy, &approvalStrategy};

using namespace eHSM;

const uint32_t IDLE_STATE_EVENTS  = 1;
const uint32_t MATCH_STATE_EVENTS = 3;

class Strategy : public HierarchicalStateMachine {
  public:
    void think() {
        bool thinkingState = this->currentState() == &(this->matchState.thinkingState);

        if (!thinkingState) {
            return;
        }

        Logging::println("[Strategy] Thinking...");

        uint8_t nextActionIndex = currentActionIndex++;

        if (nextActionIndex >= currentStrategy->size()) {
            Logging::println("[Strategy] Strategy ended.", nextActionIndex);

            nextEvent = nullptr;
            eventData = nullptr;
            return;
        }

        nextEvent = (Event*)&(currentStrategy->at(nextActionIndex).event);
        eventData = (EventData*)&currentStrategy->at(nextActionIndex).eventData;
        bool end  = currentStrategy->at(nextActionIndex).end;

        if (end) {
            Logging::println("[Strategy] Strategy ended.", nextActionIndex);
            currentActionIndex = currentStrategy->size();
            return;
        }

        char eventStr[15];

        switch (*nextEvent) {
            case Event::DoLightHouse:
                strcpy(eventStr, "DoLightHouse");
                break;
            case Event::DoGetBuoy:
                strcpy(eventStr, "DoGetBuoy");
                break;
        }

        Logging::println("[Strategy] next event : %s", eventStr);
        this->dispatch(*nextEvent);
    }

    EventData* getEventData() {
        return eventData;
    };

    Declare::IdleState<IDLE_STATE_EVENTS>   idleState;
    Declare::MatchState<MATCH_STATE_EVENTS> matchState;

    static Strategy* instance();

    void setControl(Control* control_) {
        control = control_;
    }

    void resetStrategy() {
        currentSide           = Switchers::getSide();
        uint8_t strategyIndex = Switchers::getStrategy();

        if (strategyIndex >= strategies.size()) {
            Logging::println("[Strategy] Error: strategy %i is unknown", strategyIndex);
            strategyIndex = 0;
        }

        currentStrategy    = strategies.at(strategyIndex);
        currentActionIndex = 0;

        Logging::println("[Strategy] side = %s strategy = %i %i", currentSide ? "left" : "right", strategyIndex, currentStrategy->size());
    }

    Control* control;

  private:
    Strategy() : eventData(nullptr),
                 nextEvent(nullptr),
                 currentStrategy(strategies[0]),
                 currentActionIndex(0) {
        Logging::println("[Strategy] Lodaded default strategy 0");

        idleState.addEvent(StartMatch, matchState);

        matchState.addEvent(EmergencyStop, idleState);
        matchState.addEvent(EndMatch, idleState);

        State::Action_t actionHolder;
        matchState.signalEntered.connect<Strategy, &Strategy::resetStrategy>(this);

        setInitialState(idleState);
    };

    static Strategy s_instance;

    bool          currentSide;
    StrategyType* currentStrategy;
    uint8_t       currentActionIndex;
    EventData*    eventData;
    Event*        nextEvent;
};
