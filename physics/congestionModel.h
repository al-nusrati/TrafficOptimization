#pragma once
#include "../core/Graph.h"
#include <cmath>
#include <string>
using namespace std;

class CongestionModel
{
private:
    Graph* network;
    const double alpha;   // BPR alpha parameter (default 0.15)
    const double beta;    // BPR beta parameter (default 2.0)

    void updateOneRoadCongestion(Edge* road);
    void updateOneRoadTravelTime(Edge* road);

public:
    CongestionModel(Graph* g, double a = 0.15, double b = 2.0);

    // Goes through every road and updates congestion + travel time
    void updateAllRoads();

    // Returns "Free", "Moderate", "Congested", "Severe"
    string getCongestionLabel(double congestionRatio);
};