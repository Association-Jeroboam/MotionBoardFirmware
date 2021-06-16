#pragma once

#include "ControlThread.hpp"
#include "Event.hpp"
#include "Goal.hpp"
#include <cstring>
#include <map>

template <typename TState>
class Action {
  public:
    Action(TState initialState);
    virtual void update() = 0;
    TState       getState();

    void dispatchEvent(Event event);
    void resetEvents();

  protected:
    bool   m_events[20] = {false};
    TState m_currentState;
};

template <typename TState>
Action<TState>::Action(TState initialState) : m_currentState(initialState) {}

template <typename TState>
TState Action<TState>::getState() {
    return m_currentState;
}

template <typename TState>
void Action<TState>::dispatchEvent(Event event) {
    m_events[event] = true;
    update();
}

template <typename TState>
void Action<TState>::resetEvents() {
    memset(m_events, 0, sizeof(m_events));
}
