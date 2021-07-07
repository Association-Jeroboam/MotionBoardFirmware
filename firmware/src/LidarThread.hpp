#pragma once
#include "ch.hpp"
#include "RPLidar.h"

constexpr uint16_t LIDAR_THREAD_WA = 0x400;

class LidarThread : public chibios_rt::BaseStaticThread<LIDAR_THREAD_WA> {
public:
    static LidarThread * instance();
    void main() override;

private:

    LidarThread();
    static LidarThread s_instance;

    RPLidar m_lidar;
    bool m_scanComplete;
    uint16_t m_sampleCount;
};
