#include <cmath>
#include "LidarThread.hpp"
#include "AvoidanceThread.hpp"
#include "Logging.hpp"
#include "BuildConf.hpp"
#include <new>

LidarThread LidarThread::s_instance;

LidarThread * LidarThread::instance() {
    return &s_instance;
}

LidarThread::LidarThread() : BaseStaticThread<LIDAR_THREAD_WA>() {
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

    bool timeoutDisplayed = false;
    while (!shouldTerminate()) {
        u_result res = m_lidar.waitPoint(50);

        if(res == RESULT_OK) {
            timeoutDisplayed = false;
            AvoidanceThread::instance()->sendPoint(&m_lidar.getCurrentPoint());
        } else {
            if(!timeoutDisplayed) {
                timeoutDisplayed = true;
                m_lidar.startScan();
                Logging::println("[LidarThread] wait point timeout");
            }
            chThdSleepMilliseconds(10);
        }
    }

    Logging::println("[LidarThread] Shutdown");
}
