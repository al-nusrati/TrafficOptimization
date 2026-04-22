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
	unordered_map<int, Vehicle> vehicles;
public:
	
	TrafficGenerator(Graph* g);
	void generate(int count, int maxArrivalTime = 100);
	Vehicle* getVehicle(int id)
	{
		auto it = vehicles.find(id);
		if (it != vehicles.end())
			return &(it->second);      //vehicle object
		return nullptr;
	}
	int getVehicleCount() const
	{
		return int(vehicles.size());
	}
	const unordered_map<int, Vehicle>& getVehicles() const
	{
		return vehicles;
	}
	void printVehicles();
	void printStatus();

};