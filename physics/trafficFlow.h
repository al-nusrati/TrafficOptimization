#pragma once
#include "../core/Graph.h"
#include "../core/Vehicle.h"

class TrafficFlow
{
private:
    Graph* network;

public:
    TrafficFlow(Graph* g);

    void decrementTimers();

    void updateFlowRates();

    void updateAll();
};