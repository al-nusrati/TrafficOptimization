#pragma once
#include "Graph.h"
#include "Vehicle.h"
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include<iostream>
using namespace std;
class TrafficGenerator
{
private:
	//pointer to road network
	Graph* network;
	int nextVehicleID;
public:
	unordered_map<int, Vehicle>vehicles;
	TrafficGenerator(Graph* g);
	void generate(int count, int maxArrivalTime = 100);
	Vehicle* getVehicle(int id);
	void printVehicles();
	void printStatus();


};