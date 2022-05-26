#pragma once
#include "ch.hpp"

constexpr uint32_t EMGCY_WATCHER_WA = 0x100;


class EmergencyWatcher: public chibios_rt::BaseStaticThread<EMGCY_WATCHER_WA>{
  public:
    EmergencyWatcher();
  private:
    void main();
    void sendEmgcyState();
    chibios_rt::EventListener m_listener;
    bool m_emgcy_state;
};

