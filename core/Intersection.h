#pragma once
#include<iostream>
#include<string>
#include "Edge.h"   //intersection contains edges in adjacency list
// one node in the SLL adjacency list
//intersection keeps a chain of all outgoing nodes ,one per outgoing road
struct EdgeNode {
	Edge edge;           //the actual road data
	EdgeNode* next;      //pointer to next node from this same intersection

	EdgeNode(Edge e):edge(e),next(nullptr) {}
};
//one node in teh graph
struct Intersection
{
	int id;          //unique ID for intersection
	string name;     //Name for intersection
	EdgeNode* adjList;       //adjacency list for this node ...points to first EdgeNode(first road going out)
	int outDegree;         //how many roads leave this intersection
	
	Intersection() : id(-1),name(" "),adjList(nullptr),outDegree(0) { }
	Intersection(int ID, string name) : id(ID),name(name),adjList(nullptr),outDegree(0) {}

	//insert one outgoing road to SLL
	void addEdge(Edge e)
	{
		EdgeNode* newNode = new EdgeNode(e);       //allocating a memory for new roads
		newNode->next = adjList;         
		adjList = newNode;
		outDegree++;         //outgoing roads increased
	}
	Edge* getDestination(int j)     //find road that goes to intersection j
	{
		EdgeNode* curr = adjList;    //start from head of list
		while (curr != nullptr)
		{
			if (curr->edge.destination == j)
				return &(curr->edge);    //return address of the road

			curr = curr->next;
		}
		return nullptr;
	}
	~Intersection()
	{
		EdgeNode* curr = adjList;
		while (curr != nullptr)
		{
			EdgeNode* temp = curr->next;
			delete curr;
			curr = temp;
		}
	}
};