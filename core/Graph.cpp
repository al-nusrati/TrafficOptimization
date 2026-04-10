#include "Graph.h"
#include<iostream>
using namespace std;

Graph::Graph() : totalIntersections(0),totalRoads(0) 
{ }
void Graph::addIntersection(int id, string name)
{
	if (id < 0 || id >= MAX_NODES)
	{
		cout << "Error at Instersection ID " << id << " must be between 0 and " << MAX_NODES - 1 << "\n";
		return;
	}
	if (nodeExists(id))
	{
		cout << "Intersection " << id << "already exists! " << endl;
		return;
	}
	nodes[id] = Intersection(id, name);
	totalIntersections++;
}
void Graph::addRoad(int source, int destination, double length, double maxSpeed, double capacity)
{
	//check existence
	if (!nodeExists(source))
	{
		cout << "Destination intersection " << source << "does not exits!" << endl;
		return;
	}
	if (!nodeExists(destination))
	{
		cout << "Destination intersection " << destination << "does not exits!" << endl;
		return;
	}
	if (length <= 0)
	{
		cout << "Road length must be greater than 0!" << endl;

	}
	if (maxSpeed <= 0)
	{
		cout << "Max speed ,uist be greater than 0" << endl;
		return;
	}
	if (capacity <= 0)
	{
		cout << "Capacity must be greater than 0" << endl;
		return;
	}
	//create edge and insert into intersection i's adjacency list
	Edge e(destination, length, maxSpeed, capacity);
	nodes[source].addEdge(e);      //insert this edge into intersectrion
	totalRoads++;                  //one more road in the network


}
//return pointer ti the edge(road)
Edge* Graph::getEdge(int source, int destination)
{
	if (!nodeExists(source))
		return nullptr;

	return nodes[source].getDestination(destination);
}
//node exists
bool Graph::nodeExists(int id)
{
	if (id<0 || id>MAX_NODES)
		return false;
	return nodes[id].id == id;      //a valid intersection has nodes
}
void Graph::printNetwork()
{
	cout << "====ROAD NETWORK MAP (Total Intersections:  " << totalIntersections << " ,Total Roads: " << totalRoads << " )====" << endl;
	for (int i = 0;i < MAX_NODES;i++)
	{
		if (nodes[i].id == -1)      //skip unused slots
			continue;

		cout << "Intersection [ " << nodes[i].id << "]" << "\"" << nodes[i].name << "\"" << "(Roads going out : " << nodes[i].outDegree << ")" << endl;

		//traverse this intersection's adjacency list
		EdgeNode* curr = nodes[i].adjList;
		while (curr != nullptr)
		{
			cout << "Source " << i << "_" << curr->edge.destination << " -> [" << curr->edge.destination
				<< " ]" << " \nLength of Road : " << curr->edge.roadLength << " \nMaximum Speed : " << curr->edge.maxSpeed
				<< "\n Road Capacity : " << curr->edge.capacity << "\n Travel Time when Traffic is Zero: " << curr->edge.freeTravelTime
				<< endl;

			curr = curr->next;
			//move to nect edgeNode 
		}
	}
}