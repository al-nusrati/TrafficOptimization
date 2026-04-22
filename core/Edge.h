#pragma once
#include <iostream>
#include <deque>   
using namespace std;

struct Vehicle;

struct Edge
{
private:
    double flowRate;
    double travelTime;
    double ratio;
    bool greenSignal;
    deque<Vehicle*> vehiclesOnRoad;  

public:
    double freeTravelTime;
    double capacity;
    double maxSpeed;
    double roadLength;
    int destination;
    double lastRatio;                 

    Edge() : destination(-1), roadLength(0), maxSpeed(1), capacity(1), flowRate(0), freeTravelTime(0), travelTime(0), ratio(0), greenSignal(true), lastRatio(0.0) {}

    Edge(int destination, double length, double maxSpeed, double capacity)
        : destination(destination), roadLength(length), maxSpeed(maxSpeed), capacity(capacity), flowRate(0.0), freeTravelTime(length / maxSpeed), 
		travelTime(length / maxSpeed), ratio(0.0), greenSignal(true), lastRatio(0.0) {}

    double getFlowRate()   const { return flowRate; }
    double getTravelTime() const { return travelTime; }
    double getRatio()      const { return ratio; }
    bool isGreenSignal()   const { return greenSignal; }

    void setFlowRate(double f) {
        if (f < 0) f = 0;
        flowRate = f;
        ratio = (capacity > 0) ? (flowRate / capacity) : 0;
    }

    void setTravelTime(double t) {
        if (t < freeTravelTime) t = freeTravelTime;
        travelTime = t;
    }

    void setRatio(double r) {
        if (r < 0) r = 0;
        ratio = r;
    }

    void setGreenSignal(bool g) { greenSignal = g; }

    void addVehicle(Vehicle* v) {
        if (v == nullptr) return;
        vehiclesOnRoad.push_back(v);      
        setFlowRate(flowRate + 1);
    }

    Vehicle* removeVehicle() {
        if (vehiclesOnRoad.empty()) return nullptr;
        Vehicle* v = vehiclesOnRoad.front();
        vehiclesOnRoad.pop_front();       
        setFlowRate(flowRate - 1);
        return v;
    }

    Vehicle* frontVehicle() const {
        if (vehiclesOnRoad.empty()) return nullptr;
        return vehiclesOnRoad.front();
    }

    bool isEmpty() const {
        return vehiclesOnRoad.empty();
    }

    const deque<Vehicle*>& getVehicles() const { return vehiclesOnRoad; }
    size_t vehicleCount() const { return vehiclesOnRoad.size(); }
};