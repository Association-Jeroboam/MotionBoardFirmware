#pragma once
#include "ch.hpp"
#include "RPLidar.h"

constexpr uint16_t LIDAR_THREAD_WA = 0x200;
constexpr uint16_t SCAN_SIZE = 360;


class LidarThread : public chibios_rt::BaseStaticThread<LIDAR_THREAD_WA>,
                    public chibios_rt::EventSource{
public:
    static LidarThread * instance();
    void main() override;

private:
    enum ScanDataIdx {
        ANGLE    = 0,
        DISTANCE = 1,
    };
    LidarThread();

    RPLidar m_lidar;
    bool m_scanComplete;
    uint16_t m_sampleCount;
    float m_scan[2][SCAN_SIZE];
};
