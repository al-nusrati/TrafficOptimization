#pragma once
#include "../core/Graph.h"
#include "../core/TrafficGenerator.h"
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

struct VehicleRecord{
    int vehicleID;
    double actualTravelTime;
    double freeFlowTime;
    double delay;
};

class PerformanceMetrics{
private:
    Graph* network;
    int totalVehicles;
    int currentTime;
    vector<VehicleRecord> arrivals;
    double totalCongestionAccum;
    int congestionSnapshots;

    bool alreadyRecorded(int vehicleID) const;

public:
    PerformanceMetrics(Graph* g, int total);
    void tick();
    void recordArrivals(const unordered_map<int, Vehicle>& vehicles);
    void recordCongestionSnapshot();
    void exportToJSON(const string& filename) const;

    int getArrivedCount() const;
    double getThroughput() const;
    int getCurrentTime() const;
};