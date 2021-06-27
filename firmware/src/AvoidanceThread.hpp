#pragma once
#include <inttypes.h>
#include "ch.hpp"
#include "hal.h"
#include "RPLidar.h"

constexpr uint16_t AVOIDANCE_THREAD_WA = 0x200;

constexpr uint16_t AVOIDANCE_POINTS_QUEUE_LEN  = 20;
constexpr uint16_t SCAN_SIZE = 360;

class AvoidanceThread : public chibios_rt::BaseStaticThread<AVOIDANCE_THREAD_WA> {
  public:
    static AvoidanceThread * instance();
    bool sendPoint(const RPLidarMeasurement* point);

  private:
    enum ScanDataIdx {
        ANGLE    = 0,
        DISTANCE = 1,
    };
    void main() override;
    AvoidanceThread();
    void filterPoints();
    static AvoidanceThread s_instance;

    objects_fifo_t     m_pointQueue;
    RPLidarMeasurement m_dataBuffer[AVOIDANCE_POINTS_QUEUE_LEN];
    msg_t              m_msgBuffer[AVOIDANCE_POINTS_QUEUE_LEN];
    uint16_t           m_sampleCount;

    float              m_scan[SCAN_SIZE][2];
    uint16_t           m_sortingStack[SCAN_SIZE];

};
