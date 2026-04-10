#pragma once
#include "Intersection.h"
#include<iostream>
#include<string>
using namespace std;

const int MAX_NODES = 100;    //maximum number of intersections our graph can hold is 100

class Graph
{
public:
	Intersection nodes[MAX_NODES];
	int totalIntersections;
	int totalRoads;
	Graph();

	void addIntersection(int id, string name);
	
	void addRoad(int source, int destination, double length, double maxSpeed, double capacity);
	
	Edge* getEdge(int source, int destination);
	
	bool nodeExists(int id);
	
	void printNetwork();
	

};