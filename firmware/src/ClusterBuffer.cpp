#include "ClusterBuffer.hpp"

ClusterBuffer::ClusterBuffer(uint8_t clusterLen): m_clusterLen(clusterLen) {
    m_head = 0;
    m_pointCount = 0;
}

void ClusterBuffer::addPoint(Point point, bool valid){
    if(!this->isFull()) {
        m_pointCount++;
    }
    m_pointBuffer[m_head] = point;
    m_validBuffer[m_head] = valid;
    m_head = (m_head + 1) % CLUSTER_BUFFER_SIZE;
}

Point ClusterBuffer::getClusterPos(){
    if(!this->isValid()) {
        return Point(0., 0.);
    }

    float x = 0.;
    float y = 0.;

    for (uint8_t i = 0; i < CLUSTER_BUFFER_SIZE; i++) {
        if(!m_validBuffer[i]) {
            continue;
        }

        x += m_pointBuffer[i].x();
        y += m_pointBuffer[i].y();
    }

    x /= m_pointCount;
    y /= m_pointCount;

    return Point(x, y);
}

bool ClusterBuffer::isFull(){
    return m_pointCount == CLUSTER_BUFFER_SIZE;
}

bool ClusterBuffer::isValid(){
    uint8_t validPointCount = 0;
    for(uint8_t i = 0; i < CLUSTER_BUFFER_SIZE; i++) {
        if (m_validBuffer[i]) {
            validPointCount++;
        }
    }
    return validPointCount >= m_clusterLen;
}
