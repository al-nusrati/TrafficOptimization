#include "TrafficGenerator.h"

TrafficGenerator::TrafficGenerator(Graph* g) :network(g), nextVehicleID(0)
{
	//seed the current time to random number generator
	srand(static_cast<unsigned int>(time(nullptr)));


}
//generate
void TrafficGenerator::generate(int count, int maxArrivalTime)
{
	if (network->totalIntersections < 2)
	{
		cout << "Need at least 2 intersections to generate vehicles" << endl;
		return;
	}
	//Build list of valid node IDs
	int validNodes[MAX_NODES];
	int validCount = 0;
	for (int i = 0;i < MAX_NODES;i++)
	{
		if (network->nodes[i].id != -1)//skip unused nodes
			validNodes[validCount++] = i;
	}
	int generated = 0;      
	int attempts = 0;

	while (generated < count && attempts < count * 10)
	{
		attempts++;
		//pick random source and destination
		int source = validNodes[rand() % validCount];
		int destination = validNodes[rand() % validCount];
		//if a vehicle has same source and destination travel time is 0
		if (source == destination)
			continue;
		//random time when vehicle enters the simulation
		int arrivalTime = rand() % (maxArrivalTime + 1);
		//create a new vehicle object
		Vehicle v(nextVehicleID, source, destination, arrivalTime);
		//storing in the hashTable
		vehicles[nextVehicleID] = v;
		nextVehicleID++;
		generated++;
	}
	cout << "[Traffic Generator Generated : " << generated << " vehicles!" << endl;
	

}
Vehicle* TrafficGenerator::getVehicle(int id)
{
	auto it = vehicles.find(id);
	if (it != vehicles.end())
		return &(it->second);      //vehicle object
	return nullptr;
}
void TrafficGenerator::printVehicles()
{
	cout << "  Vehicle Registry (" << vehicles.size() << " vehicles) " << endl;
	for (auto& pair : vehicles)
	{
		Vehicle& v = pair.second;
		string statusStr;
		if (v.status == MOVING)
			statusStr = "MOVING";
		else if (v.status == WAITING)
			statusStr = "WAITING";
		else
			statusStr = "ARRIVED";

		cout << " Vehicle [ " << v.vehicleID << " ] "
			<< "\nVehicle Source = " << v.vehicleSource << "\n Vehicle Destination : " << v.vehicleDestination
			<< "\n Arrival Time : " << v.arrivalTime << "\n Remaining Time: " << v.remainingTime << endl;

	}
	
}
void TrafficGenerator::printStatus()
{
	cout << "   Traffic Statistics    " << endl;
	cout << "Total Vehicles : " << vehicles.size() << endl;
	//couting departures and arrival per intersection
	int departures[MAX_NODES] = { 0 };
	int arrivals[MAX_NODES] = { 0 };

	for (auto& pair : vehicles)
	{
		departures[pair.second.vehicleSource]++;
		arrivals[pair.second.vehicleDestination]++;

	}

	cout << "	Intersection	|	Departing	|	Arriving  "<<endl;
	cout << "----------------------------------------------------" << endl;
	for (int i = 0;i < MAX_NODES;i++)
	{
		if (network->nodes[i].id == -1)
			continue;
		cout << " [ " << i << " ] " << network->nodes[i].name
			<< "					"
			<< departures[i]
			<< "					"
			<< arrivals[i] << "\n";
	}

}