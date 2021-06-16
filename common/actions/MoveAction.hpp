#pragma once

#include "Control.hpp"
#include "ControlThread.hpp"
#include "Event.hpp"
#include "Goal.hpp"
#include "actions/Action.hpp"

enum MoveState {
    IDLE,
    START,
    FIRST_ROTATION,
    MOVING_FORWARD,
    LAST_ROTATION,
    FINISHED
};

class MoveAction : Action<MoveState> {
  public:
    MoveAction(float x, float y, float theta);
    void update();
    void start();
    void stop();

  private:
    void rotate(float angle);
    void moveForward();

    float m_x;
    float m_y;
    float m_theta;
};

MoveAction::MoveAction(float x, float y, float theta) : Action<MoveState>(MoveState::IDLE), m_x(x), m_y(y), m_theta(theta) {}

void MoveAction::update() {
    if (m_currentState == MoveState::START) {
        m_currentState = MoveState::FIRST_ROTATION;
        // Calcul savant pour la première rotation
        float angle = 1;
        rotate(angle);
    } else if (m_currentState == MoveState::FIRST_ROTATION && m_events.at(Event::GOAL_FINISHED)) {
        m_currentState = MoveState::MOVING_FORWARD;
        moveForward();
        resetEvents();
    } else if (m_currentState == MoveState::MOVING_FORWARD && m_events.at(Event::GOAL_FINISHED)) {
        m_currentState = MoveState::LAST_ROTATION;
        // Calcul savant pour la dernière rotation
        float angle = 1;
        rotate(angle);
        resetEvents();
    } else if (m_currentState == MoveState::LAST_ROTATION && m_events.at(Event::GOAL_FINISHED)) {
        m_currentState = MoveState::FINISHED;
        resetEvents();
    }
}

void MoveAction::start() {
    m_currentState = MoveState::START;
}

void MoveAction::stop() {
    m_currentState = MoveState::FINISHED;
}

void MoveAction::rotate(float angle) {
    Control* control = ControlThread::instance()->getControl();
    Goal     rotateGoal(angle, 0);
    control->setCurrentGoal(rotateGoal);
}

void MoveAction::moveForward() {
    Control* control = ControlThread::instance()->getControl();
    Goal     forwardGoal(m_x, m_y, Goal::COORD);
    control->setCurrentGoal(forwardGoal);
}
