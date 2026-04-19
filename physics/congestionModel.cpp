#include "congestionModel.h"

CongestionModel::CongestionModel(Graph* g) : network(g) {}

void CongestionModel::updateAllRoads()
{
    //Go through every possible intersection slot
    for (int i = 0; i < MAX_NODES; i++)
    {
        if (network->nodes[i].id == -1) continue;

        EdgeNode* currentRoad = network->nodes[i].adjList;
        //Walk through all outgoing roads one by one
        while (currentRoad != nullptr)
        {
            //Calculate how congested this road is
            updateOneRoadCongestion(&currentRoad->edge);
            //Use that congestion to update travel time
            updateOneRoadTravelTime(&currentRoad->edge);
            //Move to next road from this intersection
            currentRoad = currentRoad->next;
        }
    }
}

void CongestionModel::updateOneRoadCongestion(Edge* road)
{
    if (road->capacity <= 0)    return;
    
    road->lastRatio = road->ratio;  
    double vehiclesOnRoad = road->flowRate;
    double roadCapacity   = road->capacity;

    road->ratio = vehiclesOnRoad / roadCapacity;

}

void CongestionModel::updateOneRoadTravelTime(Edge* road)
{
    // freeTime is already stored in edge (length / maxSpeed)
    double freeTime         = road->freeTravelTime;
    double congestionRatio  = road->ratio;

    // Apply BPR formula
    double congestionPenalty = alpha * pow(congestionRatio, beta);
    road->travelTime = freeTime * (1.0 + congestionPenalty);
}

string CongestionModel::getCongestionLabel(double congestionRatio)
{
    if (congestionRatio < 0.3)          return "Free";
    else if (congestionRatio < 0.7)     return "Moderate";
    else if (congestionRatio < 0.95)    return "Congested";
    else                                return "Severe";
}