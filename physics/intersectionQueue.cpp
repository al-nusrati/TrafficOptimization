#include "intersectionQueue.h"
#include <iostream>
using namespace std;

IntersectionQueue::IntersectionQueue(Graph* g, double rate) : network(g), dischargeRate(rate) {}

void IntersectionQueue::dischargeVehicles(int currentTime){
    for (int i = 0; i < MAX_NODES; i++){
        if (network->nodes[i].id == -1) continue;

        EdgeNode* curr = network->nodes[i].adjList;
        while (curr != nullptr){
            Edge& road = curr->edge;
            int j = road.destination;

            // Gate 1: signal must be green
            if (!road.isGreenSignal())
            {
                curr = curr->next;
                continue;
            }

            int discharged = 0;
            int maxD = static_cast<int>(dischargeRate);

            while (!road.isEmpty() && discharged < maxD)
            {
                Vehicle* v = road.frontVehicle();

                // Gate 2: front vehicle still traveling
                if (v->remainingTime > 0.0)    break;

                // Case A: reached destination
                if (j == v->vehicleDestination){
                    road.removeVehicle();              
                    v->currentNode = j;
                    v->markArrived();
                    discharged++;
                    continue;
                }

                // Case B: move to next road
                int k = v->nextNode;
                if (k == -1) break;

                Edge* nextRoad = network->getEdge(j, k);
                if (nextRoad == nullptr){
                    cout << "[Queue] WARNING: no road " << j << "->" << k << " for vehicle " << v->vehicleID << "\n";
                    break;
                }

                // Gate 3: capacity check
                if ((nextRoad->capacity - nextRoad->getFlowRate()) <= 0.0)    break;

                // All gates passed — move vehicle
                road.removeVehicle();                   
                v->currentNode   = j;
                v->nextNode      = -1;
                v->remainingTime = nextRoad->getTravelTime();   
                v->status        = MOVING;
                nextRoad->addVehicle(v);                

                discharged++;
            }

            curr = curr->next;
        }
    }
}

int IntersectionQueue::getWaitingCount(int src, int dest) const
{
    Edge* e = network->getEdge(src, dest);
    if (e == nullptr) return 0;

    int count = 0;
    for (Vehicle* v : e->getVehicles())
        if (v->remainingTime <= 0.0)    count++;
    return count;
}