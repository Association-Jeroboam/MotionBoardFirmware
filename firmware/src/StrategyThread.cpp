#include "StrategyThread.hpp"
#include "ControlThread.hpp"
#include "Events.hpp"
#include "Logging.hpp"
#include "MotionBoard.hpp"
#include <new>

enum StrategyThreadEvent {
    ComEvent     = 1 << 0,
    BoardEvent   = 1 << 1,
    ControlEvent = 1 << 2,
};

using namespace chibios_rt;

StrategyThread StrategyThread::s_instance;

StrategyThread* StrategyThread::instance() {
    return &s_instance;
}

void StrategyThread::main() {

    setName("StrategyThread");
    chThdSleep(TIME_MS2I(10));

    ControlThread::instance()->registerMask(&m_controlListener, ControlEvent);
    Board::Events::eventRegister(&m_boardListener, BoardEvent);

    Strategy::instance()->setControl(ControlThread::instance()->getControl());
    Board::Events::startStartMatchTimer(3000);
    Logging::println("[StrategyThread] init %p", this);

    while (!shouldTerminate()) {
        eventmask_t event = waitOneEvent(ControlEvent | BoardEvent);

        if (event & ComEvent) {
            Logging::println("[StrategyThread] Com Event");
        }
        if (event & BoardEvent) {
            eventflags_t flags = m_boardListener.getAndClearFlags();
            if (flags & Board::Events::START_MATCH) {
                Logging::println("[StrategyThread] StartMatch");
                Strategy::instance()->dispatch(StartMatch);
            }

            if (flags & Board::Events::END_MATCH) {
                Logging::println("[StrategyThread] EndMatch");
                Strategy::instance()->dispatch(EndMatch);
            }

            if (flags & Board::Events::COMPASS_TIMEOUT) {
                Logging::println("[StrategyThread] Compass");
                Strategy::instance()->dispatch(CompassOk);
            }
            if (flags & Board::Events::FLAG_TIMEOUT) {
                Logging::println("[StrategyThread] Flag");
                Strategy::instance()->dispatch(StartFunnyAction);
            }
        }
        if (event & ControlEvent) {
            eventflags_t flags = m_controlListener.getAndClearFlags();
            if (flags & GoalReached) {
                Logging::println("[StrategyThread] MoveOk");
                Strategy::instance()->dispatch(MoveOk);
                // Strategy::instance()->dispatch(CanMove);
            }
        }
    }

    Logging::println("[StrategyThread] end");
}

StrategyThread::StrategyThread() : BaseStaticThread<STRATEGY_THREAD_WA>() {
}
