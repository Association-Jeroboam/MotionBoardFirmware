#include "AvoidanceThread.hpp"
#include <cmath>
#include "Logging.hpp"
#include "QuickSort.h"
#include "ClusterBuffer.hpp"
#include "ControlThread.hpp"
#include "Parameters.hpp"

AvoidanceThread AvoidanceThread::s_instance;

AvoidanceThread* AvoidanceThread::instance() {
    return &s_instance;
}

AvoidanceThread::AvoidanceThread(): BaseStaticThread<AVOIDANCE_THREAD_WA>(),EventSource(), m_robotDetected(false){
    chFifoObjectInit(&m_pointQueue, sizeof(m_dataBuffer[0]), AVOIDANCE_POINTS_QUEUE_LEN,  m_dataBuffer, m_msgBuffer);
}

void AvoidanceThread::main() {
    setName("Avoidance Thread");

    Logging::println("[Avoidance Thread] Init");
    while (!shouldTerminate()) {
        RPLidarMeasurement * point;
        chFifoReceiveObjectTimeout(&m_pointQueue, (void **)&point, TIME_INFINITE);

        float angle = LIDAR_ANGLE_OFFSET - point->angle;
        if(angle < 0. || angle > 360.) {
            angle = fabsf(fmodf(angle, 360.));
        }
        angle = angle * (2. * M_PI / 360.);
        float distance = point->distance;
        if(point->startBit || (m_sampleCount >= SCAN_SIZE - 1) ) {
            if( m_sampleCount < 150) {
                m_sampleCount = 0;
                m_scan[m_sampleCount][ANGLE]    = angle;
                m_scan[m_sampleCount][DISTANCE] = distance;
                chFifoReturnObject(&m_pointQueue, point);
                continue;
            }
            // start process scan
            uint32_t start = chVTGetSystemTime();
            quickSortIterative(m_scan, m_sortingStack, 0, m_sampleCount - 1);
            filterPoints();
            uint32_t elapsed = chVTGetSystemTime() - start;
//            Logging::println("processed %u smpl in %lu us", m_sampleCount, TIME_I2US(elapsed));
            //end process scan
            m_sampleCount = 0;

        } else {
            m_sampleCount++;
        }
        m_scan[m_sampleCount][ANGLE]    = angle;
        m_scan[m_sampleCount][DISTANCE] = distance;
        chFifoReturnObject(&m_pointQueue, point);
    }
    Logging::println("[Avoidance Thread] Shutdown");
}

bool AvoidanceThread::sendPoint(const RPLidarMeasurement* point){
    bool ret = false;
    RPLidarMeasurement* pointToSend = (RPLidarMeasurement*) chFifoTakeObjectTimeout(&m_pointQueue, TIME_IMMEDIATE);
    if(pointToSend == NULL) {
        Logging::println("[Avoidance Thread] Queue full!");
    } else {
        ret = true;
        *pointToSend = *point;
        chFifoSendObject(&m_pointQueue, pointToSend);
    }
    return ret;
}

void AvoidanceThread::filterPoints() {
    ClusterBuffer buffer(3);
    uint16_t dropCount = 0;
    uint16_t loops = 0;
    bool previousRobotDetected = m_robotDetected;
    m_robotDetected = false;
    for (uint16_t i = 0; i < m_sampleCount + CLUSTER_BUFFER_SIZE - 1; i++) {
        uint16_t index = i % m_sampleCount;
        float angle = m_scan[index][ANGLE];
        float distance = m_scan[index][DISTANCE];
        loops++;
        if( distance < LIDAR_MIN_DISTANCE || distance > LIDAR_MAX_DISTANCE) {
            buffer.addPoint(Point(0,0), false);
            dropCount++;
            continue;
        }

        Point robotFrame = Point::polarToCartesian(angle, distance);
        Point mapFrame   = Point::mapToRobot(robotFrame, *ControlThread::instance()->getControl()->getRobotPose());

        if( mapFrame.x() < 0.        ||
            mapFrame.x() > MAP_MAX_X ||
            mapFrame.y() < 0.        ||
            mapFrame.y() > MAP_MAX_Y    ){
            buffer.addPoint(Point(0,0), false);
            continue;
        }
        buffer.addPoint(robotFrame, true);
        Point clusterPos = buffer.getClusterPos();
        if (clusterPos.x() != 0. && clusterPos.y() != 0.){
            m_robotDetected = true;
            Logging::println("[Avoidance] Cluster detected! %.2f %.2f", clusterPos.x(), clusterPos.y());
            break;
        }
    }

    if(!previousRobotDetected && m_robotDetected) {
        this->broadcastFlags(RobotDetected);
    } else if(previousRobotDetected & !m_robotDetected) {
        this->broadcastFlags(WayCleared);
    }
}
