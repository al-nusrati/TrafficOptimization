#pragma once
#include "../core/Vehicle.h"
#include "../core/Intersection.h"
#include <vector>
#include <climits>
#include <queue>
#include <algorithm>
using namespace std;

struct GPS
{
    virtual ~GPS() {}  // Virtual destructor for proper cleanup
    virtual int getNextNode(Intersection* nodes, int count, const Vehicle& v) = 0;  // Get immediate next step
    virtual vector<int> getFullPath(Intersection* nodes, int count, int src, int dest) = 0;  // Get complete path
};

struct DijkstraGPS : public GPS
{
    // Returns the complete shortest path from src to dest using travel time as weight
    vector<int> getFullPath(Intersection* nodes, int count, int src, int dest) override
    {
        if (src < 0 || dest < 0 || src >= count || dest >= count) return vector<int>();

        // STEP 1: Convert linked list graph to vector adjacency list (what Dijkstra expects)
        vector<vector<pair<int, int>>> adj(count);  // adj[source] = list of (destination, weight)

        for (int i = 0; i < count; i++)
        {
            if (nodes[i].id == -1) continue;
            EdgeNode* curr = nodes[i].adjList;
            while (curr)
            {
                // Use freeTravelTime (length/speed) as the weight for shortest time path
                // Multiply by 10 to preserve decimal precision when converting to int
                int weight = (int)(curr->edge.freeTravelTime * 10);
                adj[i].push_back(make_pair(curr->edge.destination, 1));
                curr = curr->next;  // Move to next road
            }
        }//INT_MAX to avoid limit issues
        
        
        // STEP 2: Run Dijkstra's algorithm with parent tracking
        vector<int> dist(count, INT_MAX); // Shortest known distance to each node
        vector<int> parent(count, -1);// Parent of each node in shortest path tree (-1 = no parent)
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;  // Min-heap

        dist[src] = 0;  // Distance to source is 0
        pq.push(make_pair(0, src));// Start from source

        while (!pq.empty())
        {
            int d = pq.top().first;   // Current distance
            int u = pq.top().second;// Current node
            pq.pop();

            if (d > dist[u]) continue;// Outdated entry, skip

            //for neighbours
            for (int i = 0; i < (int)adj[u].size(); i++)
            {
                int neighbor = adj[u][i].first;
                int weight = adj[u][i].second;

                // If we found a shorter path to neighbor, update it
                if (dist[u] + weight < dist[neighbor])
                {
                    dist[neighbor] = dist[u] + weight;
                    parent[neighbor] = u;  // Record parent for path reconstruction
                    pq.push(make_pair(dist[neighbor], neighbor));
                }
            }
        }

        //reconstructing the path from src to dest using parent pointers
        vector<int> path;
        if (dist[dest] == INT_MAX) return path;  // No path exists

        int safetyCounter = 0;//to prevent infinite looping

        // Walk backwards from destination to source
        for (int current = dest; current != -1; current = parent[current])
        {
            path.push_back(current);
            
            if (safetyCounter++ > count) 
            {
                cout << "ERROR: Infinite loop detected in Dijkstra path!" << endl;
                break; 
            }
        }

        // Reverse to get path from source to destination
        reverse(path.begin(), path.end());
        return path;
    }

    // Returns the next node to move toward destination (first step of path)
    int getNextNode(Intersection* nodes, int count, const Vehicle& v) override
    {
        vector<int> path = getFullPath(nodes, count, v.currentNode, v.vehicleDestination);
        if (path.size() < 2) return -1;  // No path or already at destination
        return path[1];  // Return the first step (index 0 is current node)
    }
};

// ── NAIVE BFS GPS (fewest hops, ignores traffic) ────────────
struct NaiveGPS : public GPS
{
    // Returns complete path with fewest number of roads (ignores travel time)
    vector<int> getFullPath(Intersection* nodes, int count, int src, int dest) override
    {
        // BFS requires: parent tracking, visited set, queue for level-order traversal
        vector<int>  parent(count, -1);  // Stores previous node in BFS tree
        vector<bool> visited(count, false);  // Tracks which nodes have been seen
        queue<int>   q;  // Queue for BFS level-order traversal

        visited[src] = true;  // Mark source as visited
        q.push(src);  // Start BFS from source

        // BFS loop: explore level by level
        while (!q.empty())
        {
            int u = q.front(); q.pop();
            if (u == dest) break;  // Found destination, stop BFS early

            // Explore all outgoing roads from current node
            EdgeNode* curr = nodes[u].adjList;
            while (curr)
            {
                int neighbor = curr->edge.destination;
                if (!visited[neighbor])  // First time seeing this node
                {
                    visited[neighbor] = true;
                    parent[neighbor] = u;  // Record how we got here
                    q.push(neighbor);
                }
                curr = curr->next;
            }
        };

        // Reconstruct path (same as Dijkstra)
        vector<int> path;
        if (!visited[dest]) return path;  // Destination unreachable
        int safetyCounter = 0;

        // Walk backwards from destination to source
        for (int current = dest; current != -1; current = parent[current])
        {
            path.push_back(current);
            if (safetyCounter++ > count) {
                cout << "ERROR: Infinite loop detected in BFS path!" << endl;
                break;
            }
        };

        // Reverse to get path from source to destination
        reverse(path.begin(), path.end());
        return path;
    }

    // Returns the next node to move toward destination (first step of path)
    int getNextNode(Intersection* nodes, int count, const Vehicle& v) override
    {
        vector<int> path = getFullPath(nodes, count, v.currentNode, v.vehicleDestination);
        if ((int)path.size() < 2) return -1;  // No path available
        return path[1];  // First step from current node
    }
};