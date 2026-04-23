#include "trafficFlow.h"
#include <iostream>
using namespace std;

TrafficFlow::TrafficFlow(Graph* g) : network(g) {}

void TrafficFlow::decrementTimers(){
    for (int i = 0; i < MAX_NODES; i++){
        if (network->nodes[i].id == -1) continue;

        EdgeNode* curr = network->nodes[i].adjList;
        while (curr != nullptr){
            for (Vehicle* v : curr->edge.getVehicles()){
                v->decrementTime();          
            }
            curr = curr->next;
        }
    }
}

void TrafficFlow::updateFlowRates(){
    for (int i = 0; i < MAX_NODES; i++){
        if (network->nodes[i].id == -1) continue;

        EdgeNode* curr = network->nodes[i].adjList;
        while (curr != nullptr){
            curr->edge.setFlowRate(static_cast<double>(curr->edge.vehicleCount()));
            curr = curr->next;
        }
    }
}

void TrafficFlow::updateAll(){
    decrementTimers();
    updateFlowRates();
}