#include "performanceMetrics.h"
#include <fstream>
#include <algorithm>
using namespace std;

PerformanceMetrics::PerformanceMetrics(Graph* g, int total) : network(g), totalVehicles(total), currentTime(0),
        totalCongestionAccum(0.0), congestionSnapshots(0) {}

void PerformanceMetrics::tick() { currentTime++; }

bool PerformanceMetrics::alreadyRecorded(int vehicleID) const
{
    for (const auto& r : arrivals)
        if (r.vehicleID == vehicleID) return true;
    return false;
}

void PerformanceMetrics::recordArrivals(const unordered_map<int, Vehicle>& vehicles)
{
    for (const auto& pair : vehicles)
    {
        const Vehicle& v = pair.second;
        if (v.status != ARRIVED) continue;
        if (alreadyRecorded(v.vehicleID)) continue;

        VehicleRecord rec;
        rec.vehicleID = v.vehicleID;
        rec.actualTravelTime = v.totalTravelTime;
        rec.freeFlowTime = v.freeTravelTime;
        rec.delay = max(0.0, v.totalTravelTime - v.freeTravelTime);
        arrivals.push_back(rec);
    }
}

void PerformanceMetrics::recordCongestionSnapshot()
{
    double sum = 0.0;
    int count = 0;
    for (int i = 0; i < MAX_NODES; i++)
    {
        if (network->nodes[i].id == -1) continue;
        EdgeNode* curr = network->nodes[i].adjList;
        while (curr)
        {
            sum += curr->edge.getRatio();
            count++;
            curr = curr->next;
        }
    }
    if (count > 0)
    {
        totalCongestionAccum += sum / count;
        congestionSnapshots++;
    }
}

void PerformanceMetrics::exportToJSON(const string& filename) const
{
    ofstream file(filename);
    if (!file.is_open()) return;

    double travelSum = 0.0, delaySum = 0.0;
    int arrived = static_cast<int>(arrivals.size());
    for (const auto& r : arrivals)
    {
        travelSum += r.actualTravelTime;
        delaySum += r.delay;
    }
    double avgTravel = (arrived > 0) ? travelSum / arrived : 0.0;
    double throughput = (currentTime > 0) ? (double)arrived / currentTime : 0.0;
    double avgCong = (congestionSnapshots > 0) 
                     ? (totalCongestionAccum / congestionSnapshots) * 100.0 : 0.0;

    file << fixed;
    file.precision(2);
    file << "{\n";
    file << "  \"simulationSteps\"    : " << currentTime << ",\n";
    file << "  \"vehiclesArrived\"    : " << arrived << ",\n";
    file << "  \"totalVehicles\"      : " << totalVehicles << ",\n";
    file << "  \"averageTravelTime\"  : " << avgTravel << ",\n";
    file << "  \"totalDelay\"         : " << delaySum << ",\n";
    file << "  \"throughput\"         : " << throughput << ",\n";
    file << "  \"averageCongestion\"  : " << avgCong << "\n";
    file << "}\n";
}

int PerformanceMetrics::getArrivedCount() const { return arrivals.size(); }
double PerformanceMetrics::getThroughput() const 
{ 
    return (currentTime > 0) ? (double)arrivals.size() / currentTime : 0.0; 
}
int PerformanceMetrics::getCurrentTime() const { return currentTime; }