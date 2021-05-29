#pragma once

#include "ch.hpp"
#include "Strategy.hpp"

constexpr uint16_t STRATEGY_THREAD_WA = 256;

enum StrategyThreadEvent {
    ComEvent     = 1 << 0,
    BoardEvent   = 1 << 1,
    ControlEvent = 1 << 2,
};


class StrategyThread : public chibios_rt::BaseStaticThread<STRATEGY_THREAD_WA>{

  public:
    static StrategyThread* instance();

    void main() override;

  private:

    chibios_rt::EventListener m_boardListener;
    chibios_rt::EventListener m_controlListener;
    explicit StrategyThread();
    void handleEvent();
};
