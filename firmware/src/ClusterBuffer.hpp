#pragma once
#include <inttypes.h>
#include "Point.hpp"

constexpr uint8_t CLUSTER_BUFFER_SIZE = 5;

class ClusterBuffer {
  public:
    ClusterBuffer(uint8_t clusterLen);
    void addPoint(Point point, bool valid);
    Point getClusterPos();

  private:
    bool isFull();
    bool isValid();

    uint8_t m_head;
    uint8_t m_pointCount;
    uint8_t m_clusterLen;
    Point   m_pointBuffer[CLUSTER_BUFFER_SIZE];
    bool    m_validBuffer[CLUSTER_BUFFER_SIZE];


};
