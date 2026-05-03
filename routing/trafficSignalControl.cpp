#pragma once

#include "../core/Edge.h"
#include <cmath>
#include<iostream>

class trafficSignalControl {
    //normally with LQF
protected:
    Edge* roads[4];     // incoming roads TO this intersection. Max 4. even though 5 here lol
    int   roadCount;    //if not 4 roads perchance
    int   greenIndex = -1; //which road has green

    //so that it doesnt stay green so one entire road is cleared:  simulator logic
    int tickCounter = 0;       // counts ticks for current green
    int sliceLength = 3;       // how many ticks before switching

public:
    trafficSignalControl(Edge* roadArray[], int count)
    {
        this->roadCount = count;//no of roads on one edge.
        for (int i = 0; i < count; i++)
            this->roads[i] = roadArray[i];//stores the number of cars on one edge in the form of an array
    }
    void updateSignal()
    {
        // reset all greens first
        for (int i = 0; i < roadCount; i++) {
            if (roads[i]) roads[i]->setGreenSignal(false);
        }

        tickCounter++;

        //if no green yet OR slice expired, pick a new road
        if (greenIndex == -1 || tickCounter >= sliceLength) 
        {
            greenIndex = selectGreen();
            tickCounter = 0; // reset slice
        }

        //set chosen road green
        if (greenIndex != -1 && roads[greenIndex]) {
            roads[greenIndex]->setGreenSignal(true);
        }
    }


    virtual ~trafficSignalControl() = default;
  bool isGreen() const 
    {
        return (greenIndex >= 0 && greenIndex < roadCount && roads[greenIndex] != nullptr);
    };
    int getGreenIndex() const { return greenIndex; }
protected:
    // Default: Longest Queue First (by vehicles on road)
    virtual int selectGreen() const 
    {
        int best = -1, maxCars = -1;
        for (int i = 0; i < roadCount; i++) {
            int cars = roads[i] ? (int)roads[i]->vehicleCount() : 0;
            //if road exists-return number of cars on it, otherwose return null pointer
            if (cars > maxCars) 
            {
                maxCars = cars;
                best = i;
            }
        }
        return best;
    }
};



///// WEIGHTED: factors in how long cars have been waiting in queue──────
class WeightedSignalControl : public trafficSignalControl 
{// with wait time 
public:
    float waitWeight = 5.0f;  //the  priority to the waiting car.

    WeightedSignalControl(Edge* roadArray[], int count)
        : trafficSignalControl(roadArray, count) {
    }

protected:
    int selectGreen() const override
    {
        int   best = -1;
        float bestScore = -1.0f;

        for (int i = 0; i < roadCount; i++) 
        {
            if (!roads[i]) continue;
            int cars = (int)roads[i]->vehicleCount();

            // waitTime not in Edge yet — use ratio as congestion proxy
            double score = cars + waitWeight * roads[i]->getRatio();
            if (score > bestScore) {
                bestScore = score;
                best = i;
            }
        }
        return best;
    }
};