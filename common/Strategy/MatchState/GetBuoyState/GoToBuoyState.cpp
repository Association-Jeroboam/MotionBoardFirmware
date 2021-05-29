#include <stdio.h>

#include "GetBuoyState.hpp"
#include "GoToBuoyState.hpp"
#include "Goal.hpp"
#include "Strategy/Strategy.hpp"


namespace eHSM {
namespace Declare {

template <std::uint32_t MAX_EVENTS_HANDLED>
GoToBuoyState<MAX_EVENTS_HANDLED>::GoToBuoyState(float x, float y) : eHSM::State(&eventList_), x(x), y(y) {
}

template <std::uint32_t MAX_EVENTS_HANDLED>
void GoToBuoyState<MAX_EVENTS_HANDLED>::onEntry() {
    Logging::println("[ENTER] GoToBuoyState\n");
    Goal goal(x, y, Goal::Direction::FORWARD);
    Logging::println("[ENTER] GoToBuoyState %f %f\n", x, y);
    Logging::println("[ENTER] Goal %f %f\n", goal.getCoordData().x, goal.getCoordData().y);
    Strategy::instance()->control->setCurrentGoal(goal);
}

template <std::uint32_t MAX_EVENTS_HANDLED>
void GoToBuoyState<MAX_EVENTS_HANDLED>::onExit() {
    Logging::println("[EXIT] GoToBuoyState\n");
}

template class GoToBuoyState<GO_TO_BUOY_STATE_EVENTS>;

} // namespace Declare
} // namespace eHSM