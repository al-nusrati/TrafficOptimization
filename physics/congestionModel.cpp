#include "congestionModel.h"

CongestionModel::CongestionModel(Graph* g, double a, double b) 
    : network(g), alpha(a), beta(b) {}

void CongestionModel::updateAllRoads()
{
    for (int i = 0; i < MAX_NODES; i++)
    {
        if (network->nodes[i].id == -1) continue;
        EdgeNode* curr = network->nodes[i].adjList;
        while (curr)
        {
            updateOneRoadCongestion(&curr->edge);
            updateOneRoadTravelTime(&curr->edge);
            curr = curr->next;
        }
    }
}

void CongestionModel::updateOneRoadCongestion(Edge* road)
{
    if (road->capacity <= 0) return;
    road->lastRatio = road->getRatio();
    road->setRatio(road->getFlowRate() / road->capacity);
}

void CongestionModel::updateOneRoadTravelTime(Edge* road)
{
    double ratio = road->getRatio();
    double penalty = alpha * pow(ratio, beta);
    road->setTravelTime(road->freeTravelTime * (1.0 + penalty));
}