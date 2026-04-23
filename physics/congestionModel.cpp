#include "congestionModel.h"

CongestionModel::CongestionModel(Graph* g, double a, double b) : network(g), alpha(a), beta(b) {}

void CongestionModel::updateAllRoads(){
    for (int intersectionID = 0; intersectionID < MAX_NODES; intersectionID++){
        if (network->nodes[intersectionID].id == -1) continue;

        EdgeNode* currentRoad = network->nodes[intersectionID].adjList;
        while (currentRoad != nullptr){
            updateOneRoadCongestion(&currentRoad->edge);
            updateOneRoadTravelTime(&currentRoad->edge);
            currentRoad = currentRoad->next;
        }
    }
}

void CongestionModel::updateOneRoadCongestion(Edge* road){
    if (road->capacity <= 0) return;
    road->lastRatio = road->getRatio();
    
    double vehiclesOnRoad = road->getFlowRate();
    double roadCapacity   = road->capacity;

    road->setRatio(vehiclesOnRoad / roadCapacity);
}

void CongestionModel::updateOneRoadTravelTime(Edge* road){
    double freeFlowTime      = road->freeTravelTime;
    double congestionRatio   = road->getRatio();

    double penalty = alpha * pow(congestionRatio, beta);
    road->setTravelTime(freeFlowTime * (1.0 + penalty));
}

string CongestionModel::getCongestionLabel(double congestionRatio){
    if (congestionRatio < 0.3)      return "Free";
    else if (congestionRatio < 0.7) return "Moderate";
    else if (congestionRatio < 0.95)return "Congested";
    else                            return "Severe";
}