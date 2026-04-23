#include "intersectionQueue.h"
#include <iostream>
using namespace std;

IntersectionQueue::IntersectionQueue(Graph* g, double rate) : network(g), dischargeRate(rate) {}

void IntersectionQueue::dischargeVehicles(int currentTime){
    for (int intersectionID = 0; intersectionID < MAX_NODES; intersectionID++){
        if (network->nodes[intersectionID].id == -1) continue;

        EdgeNode* currentRoadNode = network->nodes[intersectionID].adjList;
        while (currentRoadNode != nullptr){
            Edge& road = currentRoadNode->edge;
            int destinationNode = road.destination;

            // Gate 1: signal check
            if (!road.isGreenSignal())
            {
                currentRoadNode = currentRoadNode->next;
                continue;
            }

            int dischargedCount = 0;
            int maxDischarge = static_cast<int>(dischargeRate);

            while (!road.isEmpty() && dischargedCount < maxDischarge)
            {
                Vehicle* vehicle = road.frontVehicle();

                // Gate 2: vehicle still moving?
                if (vehicle->remainingTime > 0.0)
                    break;

                // Case A: reached final destination
                if (destinationNode == vehicle->vehicleDestination){
                    road.removeVehicle();
                    vehicle->currentNode = destinationNode;
                    vehicle->markArrived();
                    dischargedCount++;
                    continue;
                }

                // Case B: move to next road
                int nextNode = vehicle->nextNode;
                if (nextNode == -1) break;

                Edge* nextRoad = network->getEdge(destinationNode, nextNode);
                if (nextRoad == nullptr){
                    cout << "[Queue] WARNING: missing road " << destinationNode << "->" << nextNode  << " for vehicle " << vehicle->vehicleID << "\n";
                    break;
                }

                // Gate 3: capacity check
                double availableSpace = nextRoad->capacity - nextRoad->getFlowRate();
                if (availableSpace <= 0.0)
                    break;

                // Move vehicle to next road
                road.removeVehicle();
                vehicle->currentNode   = destinationNode;
                vehicle->nextNode      = -1;
                vehicle->remainingTime = nextRoad->getTravelTime();
                vehicle->status        = MOVING;
                nextRoad->addVehicle(vehicle);

                dischargedCount++;
            }

            currentRoadNode = currentRoadNode->next;
        }
    }
}

int IntersectionQueue::getWaitingCount(int src, int dest) const{
    Edge* edge = network->getEdge(src, dest);
    if (edge == nullptr) return 0;

    int waitingVehicles = 0;
    for (Vehicle* vehicle : edge->getVehicles()){
        if (vehicle->remainingTime <= 0.0)
            waitingVehicles++;
    }
    return waitingVehicles;
}