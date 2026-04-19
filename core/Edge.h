#pragma once
#include <iostream>
#include <deque>
using namespace std;

struct Vehicle;

struct Edge
{
    // ── Physical properties (never change) ───────────────────
    int    destination;
    double roadLength;
    double maxSpeed;
    double capacity;

    // ── Traffic state (updated each step) ────────────────────
    double flowRate;          // f_ij(t)  : vehicles on road right now
    double freeTravelTime;    // w_free   : length / maxSpeed
    double travelTime;        // w_ij(t)  : congestion-adjusted (BPR formula)
    double ratio;             // rho_ij   : flowRate / capacity  (0=free, 1+=jammed)
    double lastRatio;         // rho from PREVIOUS step — Person 3 uses this
                              // to decide: if |ratio - lastRatio| > 0.1 → re-run Dijkstra
                              // otherwise keep old route (no wasted Dijkstra calls)

    // ── Signal state (set by trafficSignalControl each step) ──
    bool   greenSignal;       // true = vehicles can leave this road

    // ── Vehicle queue ─────────────────────────────────────────
    // deque instead of queue → supports iteration (needed for timer decrement)
    // push_back() to enter road | pop_front() to discharge | front() to peek
    deque<Vehicle*> vehiclesOnRoad;

    // ── Constructors ──────────────────────────────────────────
    Edge()
        : destination(-1), roadLength(0), maxSpeed(1), capacity(1),
          flowRate(0), freeTravelTime(0), travelTime(0),
          ratio(0), lastRatio(0), greenSignal(false)
    { }

    Edge(int destination, double length, double maxSpeed, double capacity)
        : destination(destination), roadLength(length), maxSpeed(maxSpeed),
          capacity(capacity), flowRate(0.0),
          freeTravelTime(length / maxSpeed),
          travelTime(length / maxSpeed),
          ratio(0.0), lastRatio(0.0), greenSignal(false)
    { }
};