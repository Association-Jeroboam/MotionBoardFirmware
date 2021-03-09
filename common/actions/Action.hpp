#pragma once

#include "ControlThread.hpp"
#include "Event.hpp"
#include "Goal.hpp"
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
    std::map<Event, bool> m_events{
        {Event::START_MATCH, false},
        {Event::END_MATCH, false},
        {Event::OBSTACLE_DETECTED, false},
        {Event::GOAL_FINISHED, false}};
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
    m_events = std::map<Event, bool>{
        {Event::START_MATCH, false},
        {Event::END_MATCH, false},
        {Event::OBSTACLE_DETECTED, false},
        {Event::GOAL_FINISHED, false}};
}
