#include "trafficFlow.h"
#include <iostream>
using namespace std;

TrafficFlow::TrafficFlow(Graph* g) : network(g) {}

void TrafficFlow::decrementTimers(){
    for (int intersectionID = 0; intersectionID < MAX_NODES; intersectionID++){
        if (network->nodes[intersectionID].id == -1) continue;

        EdgeNode* currentRoad = network->nodes[intersectionID].adjList;
        while (currentRoad != nullptr){
            for (Vehicle* vehicle : currentRoad->edge.getVehicles()){
                vehicle->decrementTime();
            }
            currentRoad = currentRoad->next;
        }
    }
}

void TrafficFlow::updateFlowRates(){
    for (int intersectionID = 0; intersectionID < MAX_NODES; intersectionID++){
        if (network->nodes[intersectionID].id == -1) continue;

        EdgeNode* currentRoad = network->nodes[intersectionID].adjList;
        while (currentRoad != nullptr){
            int vehicleCount = static_cast<int>(currentRoad->edge.vehicleCount());
            currentRoad->edge.setFlowRate(vehicleCount);
            currentRoad = currentRoad->next;
        }
    }
}

void TrafficFlow::updateAll(){
    decrementTimers();
    updateFlowRates();
}