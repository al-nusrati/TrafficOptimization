#pragma once
#include "../core/Graph.h"
#include "../core/Vehicle.h"

class TrafficFlow
{
private:
    Graph* network;

public:
    TrafficFlow(Graph* g);

    // Decrement remainingTime for every vehicle on every road
    void decrementTimers();

    // Sync edge.flowRate = vehiclesOnRoad.size()
    void updateFlowRates();

    // Call both — use this in main loop
    void updateAll();
};