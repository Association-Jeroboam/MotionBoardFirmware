#include <cmath>
#include "LidarThread.hpp"
#include "Logging.hpp"
#include "BuildConf.hpp"
#include <new>

LidarThread LidarThread::s_instance;

LidarThread * LidarThread::instance() {
    return &s_instance;
}

LidarThread::LidarThread() : BaseStaticThread<LIDAR_THREAD_WA>() {
    m_scanComplete = false;
    m_sampleCount = 0;
    m_scan[ANGLE][0] = 0.;
}

void LidarThread::main() {
    setName("Lidar Thread");
    chThdSleepSeconds(1);
    Logging::println("[LidarThread] Init");
    m_lidar.begin(&LIDAR_SD_DRIVER);
    rplidar_response_device_info_t info;
    m_lidar.getDeviceInfo(info);

    Logging::println("[LidarThread] device info:");
    Logging::println("fw %u", info.firmware_version);
    Logging::println("hw %u", info.hardware_version);
    Logging::println("model %u", info.model);

    m_lidar.startScan();

    while (!shouldTerminate()) {
        m_lidar.waitPoint();

        float angle    = m_lidar.getCurrentPoint().angle;
        float distance = m_lidar.getCurrentPoint().distance;

        if(m_lidar.getCurrentPoint().startBit) {
            Logging::println("[LidarThread] process scan %u smpl", m_sampleCount);
            //TODO: process scan
            m_sampleCount = 0;
        } else {
            m_sampleCount++;
        }
        m_scan[ANGLE][m_sampleCount]    = angle;
        m_scan[DISTANCE][m_sampleCount] = distance;

    }

    Logging::println("[LidarThread] Shutdown");
}