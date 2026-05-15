#include "intersectionQueue.h"
#include <iostream>
using namespace std;

IntersectionQueue::IntersectionQueue(Graph* g, double rate)  : network(g), dischargeRate(rate) {}

void IntersectionQueue::dischargeVehicles()
{
    for (int i = 0; i < MAX_NODES; i++)//itertaning through intersections
    {
        if (network->nodes[i].id == -1) continue;
        EdgeNode* currRoad = network->nodes[i].adjList; 
        while (currRoad)//iterating through roads from that intersection 
        {
            Edge& road = currRoad->edge;
            int dest = road.destination;

            if (!road.isGreenSignal())
            {
                currRoad = currRoad->next;
                continue;
            }

            int discharged = 0;
            int maxDischarge = static_cast<int>(dischargeRate);

            while (!road.isEmpty() && discharged < maxDischarge)
            {
                Vehicle* v = road.frontVehicle();
                if (v->remainingTime > 0.0) break;  //front vehicle cant be discharged yet

                if (dest == v->vehicleDestination)  
                {
                    road.removeVehicle();
                    v->currentNode = dest;
                    v->markArrived();
                    discharged++;
                    continue;
                }

                int next = v->nextNode;
                if (next == -1) break;

                Edge* nextRoad = network->getEdge(dest, next);
                if (!nextRoad) break;

                if (nextRoad->capacity - nextRoad->getFlowRate() <= 0.0) break;

                road.removeVehicle();
                v->currentNode = dest;
                v->nextNode = -1;
                v->remainingTime = nextRoad->getTravelTime();
                v->status = MOVING;
                nextRoad->addVehicle(v);
                discharged++;
            }
            currRoad = currRoad->next;
        }
    }
}

int IntersectionQueue::getWaitingCount(int src, int dest) const
{
    Edge* e = network->getEdge(src, dest);
    if (!e) return 0;
    int count = 0;
    for (Vehicle* v : e->getVehicles())
        if (v->remainingTime <= 0.0) count++;
    return count;
}