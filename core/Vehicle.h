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

	int getVehicleID() const
	{
		return vehicleID;
	}
	int getSource() const
	{
		return vehicleSource;
	}
	int getDestination() const
	{
		return vehicleDestination;
	}
	int getCurrentNode() const
	{
		return currentNode;
	}
	int getNextNode() const
	{
		return nextNode;
	}
	double getRemainingTime() const
	{
		return remainingTime;
	}
	int getArrivalTime() const
	{
		return arrivalTime;
	}
	double getTotalTravelTime() const
	{
		return totalTravelTime;
	}
	double getFreeTravelTime() const
	{
		return freeTravelTime;
	}
	void setCurrentNode(int node)
	{
		currentNode = node;
	}
	void setNextNode(int node)
	{
		nextNode = node;
	}
	void setFreeTravelTime(double time)
	{
		if (time < 0)
			time = 0;
		freeTravelTime = time;
	}
	void addTravelTime(double time)
	{
		if (time < 0)
			time = 0;
		totalTravelTime += time;
	}
	void setRemainingTime(double time)
	{
		if (time < 0)
			time = 0;
		remainingTime = time;
	}
	bool isMoving() const 
	{
		return status == MOVING;
	}
	bool isWaiting() const
	{
		return status == WAITING;
	}
	bool isArrived() const
	{
		return status == ARRIVED;
	}

	void decrementTime()
	{
		if (status != MOVING)
			return;
		remainingTime -= 1.0; //decrement by 1 time unit
		totalTravelTime += 1.0;
		if(remainingTime<0)
			remainingTime = 0;
	}

	void markArrived()
	{
		status = ARRIVED;
		
	}
	void markMoving(double roadTravelTime)
	{
		status = MOVING;
		remainingTime = roadTravelTime;
	}

	void markWaiting()
	{
		status = WAITING;
		totalTravelTime += 1.0;
	}
	bool finishedRoad() const
	{
		return (status==MOVING && remainingTime <= 0.0);
	}

};