#pragma once
#include "../core/Graph.h"
#include "../core/Vehicle.h"

class IntersectionQueue
{
private:
    Graph* network;
    double dischargeRate;//pop from road -> pass through intersection -> add to next road

public:
    IntersectionQueue(Graph* g, double rate = 3.0);   
    void dischargeVehicles();   
    int getWaitingCount(int src, int dest) const;
};