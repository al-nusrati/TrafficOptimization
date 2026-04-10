#pragma once
#include<iostream>
#include<queue>   
using namespace std;

//edge needs a vehicle queue to store the vehicles on the edge and this struct will be defined later
struct Vehicle; 

struct Edge
{
	int destination;               //the destination node of the edge 
	double roadLength;           //length of the road
	double maxSpeed;     //maximum speed allowed on the road
	double capacity;       //road capacity
	double flowRate;       //vehicles currently on the road
	double freeTravelTime;     //travel time when traffic is zero
	double travelTime;         //travel time on the road
	double ratio;       //ratio of vehicles currently on the road to the capacity of road
	queue<Vehicle*> Q_ij; //queue for the vehicles on the road

	Edge():destination(-1),roadLength(0),maxSpeed(1),capacity(1),flowRate(0),freeTravelTime(0),travelTime(0),ratio(0)
	{ }


	Edge(int destination,double length,double maxSpeed,double capacity)
		:destination(destination),roadLength(length),maxSpeed(maxSpeed),capacity(capacity),flowRate(0.0),freeTravelTime(length/maxSpeed),travelTime(length/maxSpeed), ratio(0.0) {}

	

};