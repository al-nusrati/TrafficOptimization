#pragma once
#include "../core/Graph.h"
#include "../core/Vehicle.h"

class IntersectionQueue
{
private:
    Graph* network;
    double dischargeRate;

public:
    IntersectionQueue(Graph* g, double rate = 3.0);
    void dischargeVehicles();   // no currentTime
    int getWaitingCount(int src, int dest) const;
};