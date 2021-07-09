#ifndef __ACTUATORS_HPP__
#define __ACTUATORS_HPP__

#include <inttypes.h>



namespace Actuators {
    namespace FrontPliers {
        enum ID {
            FAR_LEFT,
            LEFT,
            RIGHT,
            FAR_RIGHT,
        };
        void close(enum FrontPliers::ID id);
        void open(enum FrontPliers::ID id);
    }

    namespace BackPliers {
        enum ID {
            FAR_RIGHT,
            RIGHT,
            MIDDLE,
            LEFT,
            FAR_LEFT,
        };
        void open(enum BackPliers::ID id);
        void close(enum BackPliers::ID id);
        void engage();
        void disengage();

    }

    namespace Arms {
        enum ID {
            LEFT,
            RIGHT,
        };
        void activate(enum ID id);
        void deactivate(enum ID id);
    }
}

#endif /* __ACTUATORS_HPP__ */
