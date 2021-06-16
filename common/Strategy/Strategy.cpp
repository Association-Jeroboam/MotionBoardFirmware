#include "Strategy/Strategy.hpp"

Strategy Strategy::s_instance;

Strategy* Strategy::instance() {
    return &s_instance;
}
