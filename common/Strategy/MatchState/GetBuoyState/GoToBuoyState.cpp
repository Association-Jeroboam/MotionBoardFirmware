#include <stdio.h>

#include "GetBuoyState.hpp"
#include "GoToBuoyState.hpp"
#include "Goal.hpp"
#include "Strategy/Strategy.hpp"

namespace eHSM {
namespace Declare {

template <std::uint8_t MAX_EVENTS_HANDLED>
GoToBuoyState<MAX_EVENTS_HANDLED>::GoToBuoyState() : eHSM::State(&eventList_) {
}

template <std::uint8_t MAX_EVENTS_HANDLED>
void GoToBuoyState<MAX_EVENTS_HANDLED>::onEntry() {
    GetBuoyData*  data = (GetBuoyData*)Strategy::instance()->getEventData();
    const int16_t x    = data->x;
    const int16_t y    = data->y;

    Logging::println("[ENTER] GoToBuoyState %i %i\n", x, y);

    Goal goal(x, y, Goal::Direction::FORWARD);

    Logging::println("[DEBUG] Set Goal %i %i\n", goal.getCoordData().x, goal.getCoordData().y);

    Strategy::instance()->control->setCurrentGoal(goal);
}

template <std::uint8_t MAX_EVENTS_HANDLED>
void GoToBuoyState<MAX_EVENTS_HANDLED>::onExit() {
    Logging::println("[EXIT] GoToBuoyState\n");
}

template class GoToBuoyState<GO_TO_BUOY_STATE_EVENTS>;

} // namespace Declare
} // namespace eHSM
