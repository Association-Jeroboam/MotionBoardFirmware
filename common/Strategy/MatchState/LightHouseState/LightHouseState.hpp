#pragma once
#include "Logging.hpp"

#include "State.hpp"

namespace eHSM {
namespace Declare {
class LightHouseState : public ::eHSM::State {
  public:
    LightHouseState() : eHSM::State(&eventList_) {
        State::Action_t actionHolder;
        actionHolder.bind<LightHouseState, &LightHouseState::onCompassOk>(this);
        this->addEvent(CompassOk, actionHolder);
    }

    void onCompassOk(int event) {
        Logging::println("compass ok !!!");

        if (!compassOk) {
            Logging::println("going to do stuff !");
        }
    }

    void setCompassOk(int event) {
        Logging::println("set compass ok");
        compassOk = true;
    }

  private:
    void doStuff() {
        Logging::println("DOING STUFF BECAUSE COMPASS OK");
    }

    void onEntry() {
        Logging::println("[ENTER] LightHouseState\n");

        if (compassOk) {
            doStuff();
        }
    }

    void onExit() {
        Logging::println("[EXIT] LightHouseState\n");
    }

    bool                      compassOk;
    Declare::Array<Event, 1U> eventList_;
    E_DISABLE_COPY(LightHouseState);
};
} // namespace Declare

} // namespace eHSM
