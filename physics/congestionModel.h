#pragma once
#include "../core/Graph.h"
#include <cmath>
#include <string>
using namespace std;

class CongestionModel
{
private:
    Graph* network;
    const double alpha;
    const double beta;

    void updateOneRoadCongestion(Edge* road);
    void updateOneRoadTravelTime(Edge* road);

public:
    CongestionModel(Graph* g, double a = 0.15, double b = 2.0);
    void updateAllRoads();
};