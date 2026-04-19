#include "Graph.h"
#include <cmath>
#include <string>

class CongestionModel
{
private:
    Graph *network;
    const double alpha;  // BPR alpha parameter (default 0.15)
    const double beta;   // BPR beta parameter (default 4.0)
    void updateOneRoadCongestion(Edge* road);
    void updateOneRoadTravelTime(Edge* road);

public:
    CongestionModel(Graph* g);

    // Goes through every road and updates congestion + travel time
    void updateAllRoads();

    // Returns "Free", "Moderate", "Congested", or "Severe"
    string getCongestionLabel(double congestionRatio);
};