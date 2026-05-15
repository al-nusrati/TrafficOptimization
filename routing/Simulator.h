#pragma once
#include <vector>
#include <unordered_map>
#include "../core/Graph.h"
#include "../core/Vehicle.h"
#include "routingEngine.cpp"
#include "trafficSignalControl.cpp"
#include "../physics/CongestionModel.h"
#include "../physics/IntersectionQueue.h"
#include "../physics/PerformanceMetrics.h"
#include "../physics/trafficFlow.h"
using namespace std;

class Simulator
{
private:
    Graph* graph;
    unordered_map<int, Vehicle>   vehicles;       // vehicleID →21 Vehicle
    vector<trafficSignalControl*> signals;         // one per intersection
    unordered_map<int, GPS*>      carNavigations;  // vehicleID → GPS
    int                           currentTime;


    // -- 4 subsystems:
    CongestionModel    congestionModel;   // updates ratio + travelTime per edge
    TrafficFlow        trafficFlow;       // decrements timers + syncs flowRate
    IntersectionQueue  intersectionQueue; // moves vehicles between roads
    PerformanceMetrics metrics;           // records arrivals + congestion

    //-- Navigation Systems:
    DijkstraGPS dijkstraGPS; //to see difference between the two
    NaiveGPS    naiveGPS;
    
    //--For Gui:
    unordered_map<int, trafficSignalControl*> intersectionSignals;  
public:
    Simulator(Graph* g, int totalVehicles) :
        graph(g),
        currentTime(0),
        congestionModel(g),           // BPR alpha=0.15 beta=2 by default
        trafficFlow(g),
        intersectionQueue(g),
        metrics(g, totalVehicles)
    { };

    ~Simulator()
    {
        for (int i = 0; i < (int)signals.size(); i++)
            delete signals[i];
    }
    int gettime(){return currentTime;}
    // sets up signals for each intersection available
    void setupSignals()
    {
        for (int i = 0; i < MAX_NODES; i++)
        {
            if (graph->nodes[i].id == -1) continue;
            Edge* incoming[4];
            int count = 0;
            // find all edges whose destination == this intersection
            for (int j = 0; j < MAX_NODES; j++)
            {
                if (graph->nodes[j].id == -1) continue;
                EdgeNode* curr = graph->nodes[j].adjList;
                while (curr && count < 4)
                {
                    if (curr->edge.destination == graph->nodes[i].id)
                        incoming[count++] = &curr->edge;
                    curr = curr->next;
                }
            }
            if (count > 0)
            {
                trafficSignalControl* sig = new trafficSignalControl(incoming, count);
                signals.push_back(sig);
                intersectionSignals[graph->nodes[i].id] = sig;  // store for GUI
            };
        }
    }

    void spawnVehicle(int id, int source, int dest, int spawnTime, bool useDijkstra)
    {
        vehicles[id] = Vehicle(id, source, dest, spawnTime);
        carNavigations[id] = useDijkstra ? (GPS*)&dijkstraGPS : (GPS*)&naiveGPS;
    }

    unordered_map<int, Vehicle> getAllVehicles(){return vehicles;}//for UI 

    int getCurrentTime() { return currentTime; } // added for raylib debug
    
    trafficSignalControl* getIntersectionSignal(int id) 
    {
        auto it = intersectionSignals.find(id);
        if (it != intersectionSignals.end())
        {    return it->second; }  // pointer to the signal control
        return nullptr;          // no signal at this intersection
    };
    // Core Simulator. The ticking system
    void tick()
    {   // every signal looks at its incoming roads, picks the one with the longest queue and sets its greenSignal = true.
        // All other incoming roads get greenSignal = false.
        for (int i = 0; i < (int)signals.size(); i++)
            signals[i]->updateSignal();

        // STEP 2: SPAWN VEHICLES
        for (auto& pair : vehicles)
        {
            Vehicle& v = pair.second; //to get car address
            if (v.status == ARRIVED) continue;
            if (v.arrivalTime > currentTime) continue;  // dont move this vehicle untill its time for it to spawn

            if (v.status == WAITING)
            {
                if (v.nextNode == -1)
                {
                    v.nextNode = carNavigations[v.vehicleID]->getNextNode(graph->nodes, MAX_NODES, v);
                }
                if (v.nextNode == -1) continue;  // no route found

                Edge* road = graph->getEdge(v.currentNode, v.nextNode);
                if (!road) continue;

                if (road->isGreenSignal())
                {
                    road->addVehicle(&v);
                    v.remainingTime = road->getTravelTime();
                    v.status = MOVING;
                }
            }
        }

        // STEP 3: timer logic
        trafficFlow.updateAll();

        // STEP 4: DISCHARGE VEHICLES
        // iff vehicle reached its final destination== mark ARRIVED
        // otherwise, move it onto the next road (uses v->nextNode)
        // update nextNode to next hop BEFORE discharge runs
        // discharge uses v->nextNode to find the onward road after arrival
        for (auto& pair : vehicles)
        {
            Vehicle& v = pair.second;
            if (v.status != MOVING || v.remainingTime > 0) continue;
            // vehicle is finishing road — GPS needs to give next hop FROM arrival point
            int savedCurrent  = v.currentNode;
            v.currentNode     = v.nextNode;   // pretend already arrived
            v.nextNode        = carNavigations[v.vehicleID]->getNextNode(graph->nodes, MAX_NODES, v);
            v.currentNode     = savedCurrent; // restore so discharge can clean up correctly
        }
        intersectionQueue.dischargeVehicles();


        // STEP 5: CongestionModel and BPR formula:
        // ratio     = flowRate / capacity
        // travelTime = freeTravelTime * (1 + alpha * ratio^beta)
        // Higher traffic = higher ratio = longer travel time next tick
        congestionModel.updateAllRoads();

        // STEP 6:
        // PerformanceMetrics checks which vehicles just arrived,
        // and taking their records 
        metrics.recordArrivals(vehicles);
        metrics.recordCongestionSnapshot();
        metrics.tick();

        currentTime++;
    }

    void run(int steps)
    {
        for (int i = 0; i < steps; i++)
        {  tick();
            cout<<"\ntick t="<<steps<<endl;
        }
    };
    
    Graph* getSimGraph(){ return (this->graph); }
    
    void printStats() //for debugging! 
    {
        cout << "\n=== Simulation Results (t=" << currentTime << ") ===\n";
        for (auto& pair : vehicles)
        {
            Vehicle& v = pair.second;
            cout << "Vehicle " << v.vehicleID
                << " | GPS: " << (carNavigations[v.vehicleID] == &dijkstraGPS ? "Dijkstra" : "Naive")
                << " | Status: " << (v.status == ARRIVED ? "ARRIVED" : "IN TRANSIT")
                << " | Total time: " << v.totalTravelTime << "\n";
        }
        // export full JSON report
        metrics.exportToJSON("results.json");
    }
};

//for raylib: https://www.youtube.com/watch?v=O96m52ZxQcw