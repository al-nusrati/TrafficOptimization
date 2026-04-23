#pragma once
#include "../core/Graph.h"
#include "../core/Vehicle.h"

//  Discharges vehicles from edges when signal is green
class IntersectionQueue{
private:
    Graph* network;
    double dischargeRate;       // max vehicles discharged per edge per step

public:
    IntersectionQueue(Graph* g, double dischargeRate = 3.0);

    void dischargeVehicles(int currentTime);

    // How many vehicles on edge src->dest have remainingTime == 0
    int getWaitingCount(int src, int dest) const;
};