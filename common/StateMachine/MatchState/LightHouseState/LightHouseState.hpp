#pragma once
#include <stdio.h>

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
        printf("compass ok !!!");

        if (!compassOk) {
            printf("going to do stuff !");
        }
    }

    void setCompassOk(int event) {
        printf("set compass ok");
        compassOk = true;
    }

  private:
    void doStuff() {
        printf("DOING STUFF BECAUSE COMPASS OK");
    }

    void onEntry() {
        printf("[ENTER] LightHouseState\n");

        if (compassOk) {
            doStuff();
        }
    }

    void onExit() {
        printf("[EXIT] LightHouseState\n");
    }

    bool                      compassOk;
    Declare::Array<Event, 1U> eventList_;
    E_DISABLE_COPY(LightHouseState);
};
} // namespace Declare

} // namespace eHSM
