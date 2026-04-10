#pragma once
#include "Intersection.h"
#include <iostream>
#include <string>
//Three states a vehicle can be in
enum VehicleStatus
{
	MOVING,
	WAITING,
	ARRIVED
};
struct Vehicle
{
	int vehicleID;
	int vehicleSource;
	int vehicleDestination;
	int currentNode;     //intersection it currently passed through
	int nextNode;        //next intersection on the planned route set by Dijikstra
	double remainingTime;  //remaining time on the current road
	int arrivalTime;        
	VehicleStatus status;  //current state
	double totalTravelTime;        //time from departure to arrival
	double freeTravelTime;        //travel time this vehicle would take with zero traffic


	Vehicle() :vehicleID(-1),vehicleSource(-1),vehicleDestination(-1),currentNode(-1),nextNode(-1),
		remainingTime(0.0),arrivalTime(0),status(WAITING),totalTravelTime(0.0),freeTravelTime(0.0) {}
	Vehicle(int vehicleID, int source, int dest, int time)
		: vehicleID(vehicleID), vehicleSource(source), vehicleDestination(dest), currentNode(source),
		nextNode(-1), remainingTime(0.0), arrivalTime(time), status(WAITING), totalTravelTime(0.0), freeTravelTime(0.0)
	{ }



};