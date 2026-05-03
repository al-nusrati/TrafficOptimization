#include "raylib.h"
#include "Simulator.h"
#include "../core/TrafficGenerator.h"
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>

// Helper to give Intersections a place on the screen
struct ScreenPos { float x; float y; };
std::map<int, ScreenPos> nodePositions;

void SetupVisualGraph() {
    nodePositions[0] = { 200, 320 };//right, down
    nodePositions[1] = { 560, 330 };
    nodePositions[2] = { 550, 100 };
    nodePositions[3] = { 850, 320 };
    nodePositions[4] = { 850, 550 };
    nodePositions[5] = { 999, 320 };
    nodePositions[6] = { 560, 550 };
    nodePositions[7] = { 250, 550 };
    nodePositions[8] = { 250, 900 };
    nodePositions[9] = { 850, 900 };
    
}

int main() 
{
    srand(time(NULL)); // Seed random generator
    
    // Initialize Raylib
    const int screenWidth = 1000;
    const int screenHeight = 1000;
    InitWindow(screenWidth, screenHeight, "Traffic Simulator - DSP3");
    Texture2D background = LoadTexture("C:/Users/muham/Downloads/roadPicture.png");
    if (background.id == 0) {
        cout << "Failed to load background image!\n";
        background = LoadTextureFromImage(GenImageColor(screenWidth, screenHeight, DARKGRAY));
    }

    // Initialize Graph
    Graph myGraph;
    myGraph.addIntersection(0, "A");
    myGraph.addIntersection(1, "B");
    myGraph.addIntersection(2, "C");
    myGraph.addIntersection(3, "D");
    myGraph.addIntersection(4, "E");
    myGraph.addIntersection(5, "F");
    myGraph.addIntersection(6, "G");
    myGraph.addIntersection(7, "H");
    myGraph.addIntersection(8, "I");

    myGraph.addRoad(0, 1, 100, 10, 10);
    myGraph.addRoad(1, 0, 100, 10, 10);
    myGraph.addRoad(1, 2, 100, 10, 10);
    myGraph.addRoad(2, 1, 100, 10, 10);
    myGraph.addRoad(1, 3, 100, 10, 10);
    myGraph.addRoad(3, 4, 100, 10, 10);
    myGraph.addRoad(3, 5, 100, 10, 10);
    myGraph.addRoad(1, 6, 100, 10, 10);
    myGraph.addRoad(6, 7, 100, 10, 10);
    myGraph.addRoad(4, 7, 100, 10, 10);
    myGraph.addRoad(7, 8 , 100, 10, 10);

    SetupVisualGraph();
    Simulator sim(&myGraph, 0);
    sim.setupSignals();

    // UI State
    char inputBuffer[5] = "\0";
    int charCount = 0;
    bool isRunning = false;
    bool randomMode = false;
    int totalVehicles = 0;

    char destInput[3] = "3";//useless var.
    int desti = 6; //for default cars (except edge-less)

    SetTargetFPS(60);
    cout << "Setup Complete. Press R for random mode, O to reset.\n";

    int tickCount = 0;
    unordered_map<int, float> visualRemaining;

    while (!WindowShouldClose()) 
    {
        // ---------------INPUT HANDLING --------------------
        // Toggle random mode with R (works even when running)
        if (IsKeyPressed(KEY_R)) {
            randomMode = !randomMode;
            cout << "Random mode: " << (randomMode ? "ON" : "OFF") << "\n";
        }
       
            // Reset with O
        if (IsKeyPressed(KEY_O)) 
        {
            cout << "Resetting simulation...\n";
            isRunning = false;
            tickCount = 0;
            totalVehicles = 0;
            charCount = 0;
            inputBuffer[0] = '\0';
            visualRemaining.clear();
            // not removing vehicles on reset- they'll be overwritten in next loop anyways
        };
        
        if (!isRunning) 
        { TrafficGenerator randomCar(&myGraph);

            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= '0') && (key <= '9') && (charCount < 4))
                    inputBuffer[charCount++] = (char)key;
                key = GetCharPressed();
            }
        };
        if (IsKeyPressed(KEY_BACKSPACE) && charCount > 0) inputBuffer[--charCount] = '\0';
                    
        // Get destination digit (0-9) //ignore this, using hardcoded value desti instead!
        static char destInput = '3';
        int destKey = GetCharPressed();
        if (destKey >= '0' && destKey <= '9') destInput = (char)destKey;

        if (IsKeyPressed(KEY_ENTER) && charCount > 0) 
        {
            totalVehicles = std::stoi(inputBuffer);
            int dest = destInput - '0';
            cout << "Spawning " << totalVehicles << " vehicles to destination " << dest << "\n";
            
            if(randomMode)
            {
                for (int i = 0; i < totalVehicles+1; i++) {
                    int src = rand() % 6;
                    int dst = rand() % 6;
                    while (dst == src) 
                        {dst = rand() % 6;
                        int spawnTime = rand() % (totalVehicles * 5);
                        sim.spawnVehicle(i, src, dst, spawnTime, true);
                        }
                }
            }
            else
            {
                for (int i = 0; i < totalVehicles; i++) 
                {
                    sim.spawnVehicle(i, 0, desti, i, true);
                }
            }
            isRunning = true;
        }
        // ------------------ SIMULATION UPDATE ----------------
        if (isRunning) 
        {
            if(tickCount % 10 == 0) 
            {
                sim.tick();
                
                // Resync visual state
                for (auto& car : sim.getAllVehicles()) {
                    if (car.second.status == MOVING || car.second.status == ARRIVED)
                        visualRemaining[car.first] = (float)car.second.remainingTime;
                    else if (car.second.status == WAITING)
                        visualRemaining[car.first] = 0.0f;
                }
                
                // Clean up visualRemaining
                vector<int> toRemove;
                for (auto& visual : visualRemaining) {
                    if (sim.getAllVehicles().find(visual.first) == sim.getAllVehicles().end())
                        toRemove.push_back(visual.first);
                }
                for (int id : toRemove) visualRemaining.erase(id);
                
                // CHECK IF ALL VEHICLES HAVE ARRIVED
                if (totalVehicles > 0) {
                    int arrived = 0;
                    for (auto& car : sim.getAllVehicles()) {
                        if (car.second.status == ARRIVED) arrived++;
                    }
                    if (arrived == totalVehicles) {
                        cout << "All " << totalVehicles << " vehicles have arrived! Simulation stopped.\n";
                        isRunning = false;
                    }
                }
            }
            
            tickCount++;

            //to sync car gps
            for (auto& car : sim.getAllVehicles()) 
            {
                visualRemaining[car.first] = (float)car.second.remainingTime;
             }
            // Decrement visual timers
            for (auto& car : sim.getAllVehicles()) {
                if (car.second.status == MOVING) {
                    auto it = visualRemaining.find(car.first);
                    if (it != visualRemaining.end()) {
                        it->second -= 1.0f / 10.0f;
                        if (it->second < 0.0f) it->second = 0.0f;
                    }
                }
            }
        }
//remaining
        // ----------------- DRAWING ---------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(background, -200, -80, WHITE);

        // UI Panel
        DrawRectangle(0, 0, 200, screenHeight, DARKGRAY);
        DrawText("TRAFFIC CTRL", 20, 20, 20, GOLD);
        DrawText("Enter Car Count:", 20, 70, 15, WHITE);
        DrawRectangle(20, 95, 160, 35, LIGHTGRAY);
        DrawText(inputBuffer, 30, 100, 25, BLACK);
       
        if (!isRunning) 
        {
            DrawText("PRESS ENTER\nTO START", 20, 150, 18, GREEN);
            DrawText(TextFormat("Random Mode: %s (R)", randomMode ? "ON" : "OFF"), 20, 190, 15, randomMode ? GREEN : RED);
            DrawText("Press O to Reset", 20, 215, 15, SKYBLUE);
        } 
        else 
        {
            DrawText("SIMULATING...", 20, 150, 18, SKYBLUE);
            int arrived = 0;
            for (auto& car : sim.getAllVehicles()) if (car.second.status == ARRIVED) arrived++;
            DrawText(TextFormat("Progress: %d/%d", arrived, totalVehicles), 20, 190, 15, YELLOW);
        }

        // Draw roads
        for (int i = 0; i < MAX_NODES; i++) 
        {
            if (!myGraph.nodeExists(i)) continue;
            EdgeNode* curr = myGraph.nodes[i].adjList;
            while (curr) {
                Vector2 start = { nodePositions[i].x, nodePositions[i].y };
                Vector2 end = { nodePositions[curr->edge.destination].x, nodePositions[curr->edge.destination].y };
                DrawLineEx(start, end, 3.0f, DARKGRAY);
                curr = curr->next;
            }
        }

        // Draw vehicles
        for (auto car : sim.getAllVehicles()) 
        {
            Vector2 destPos = { nodePositions[car.second.getDestination()].x, nodePositions[car.second.getDestination()].y };
            
            if (car.second.status == ARRIVED) {
                DrawRectangleLines(destPos.x, destPos.y, 30, 30, BLACK);
                int cols = 3, rows = 3;
                int cellW = 30 / cols, cellH = 30 / rows;
                int spotCount = cols * rows;
                int spotIndex = car.second.getVehicleID() % spotCount;
                int col = spotIndex % cols, row = spotIndex / cols;
                float x = destPos.x + col * cellW + cellW/2;
                float y = destPos.y + row * cellH + cellH/2;
                DrawCircle(x, y, 4.0f, BLUE);
                continue;
            }

            if (car.second.status == MOVING) 
            {
                int drawNext = (car.second.nextNode != -1) ? car.second.nextNode : car.second.vehicleDestination;
                Edge* road = sim.getSimGraph()->getEdge(car.second.getCurrentNode(), drawNext);
                if (road) {
                    float spacing = 20.0f - (15.0f * (float)road->getRatio());
                    float offset = (float)(car.first) * spacing;
                    float totalRoadTime = (float)road->getTravelTime();
                    float t = (totalRoadTime > 0.0f) ? (1.0f - (visualRemaining[car.first] / totalRoadTime)) : 1.0f;
                    Vector2 pos = {
                        nodePositions[car.second.getCurrentNode()].x + (nodePositions[drawNext].x - nodePositions[car.second.getCurrentNode()].x) * t + offset,
                        nodePositions[car.second.getCurrentNode()].y + (nodePositions[drawNext].y - nodePositions[car.second.getCurrentNode()].y) * t
                    };
                    DrawCircleV(pos, 5.0f, GREEN);
                }
            }
            else if (car.second.status == WAITING) 
            {
                int targetNode = (car.second.nextNode != -1) ? car.second.nextNode : car.second.currentNode;
                Vector2 pos = { nodePositions[targetNode].x, nodePositions[targetNode].y };
                DrawCircleV(pos, 5.0f, ORANGE);
            }
        };

       /* for (int i = 0; i < 6; i++) 
        {
            bool green = true;
            trafficSignalControl* sig = sim.getIntersectionSignal(i);
            if (sig) green = !(sig->isGreen());
            Color c = green ? GREEN : RED;
            float w = 12;
            Vector2 pos = { nodePositions[i].x, nodePositions[i].y };
            DrawRectangle(pos.x - w/2, pos.y - w/2, w, w, c);
            DrawRectangleLines(pos.x - w/2, pos.y - w/2, w, w, BLACK);
        }*/
       
        // DRAW TRAFFIC LIGHTS - 4 rectangles around each intersection
        for (int i = 0; i < 10; i++) {
            Vector2 pos = { nodePositions[i].x, nodePositions[i].y };
            float offset = 25;
            
            // Directions: 0=Top,1=Right,2=Bottom,3=Left
            Vector2 dirs[4] = {{0, -offset}, {offset, 0}, {0, offset}, {-offset, 0}};
            Color colors[4] = {RED, RED, RED, RED};
            
            trafficSignalControl* sig = sim.getIntersectionSignal(i);
            if (sig) {
                int greenIdx = sig->getGreenIndex();
                if (greenIdx >= 0 && greenIdx < 4) colors[greenIdx] = GREEN;
            }
            
            // Draw all 4 rectangles
            for (int d = 0; d < 4; d++) 
            {
                DrawRectangle(pos.x + dirs[d].x - 6, pos.y + dirs[d].y - 6, 12, 12, colors[d]);
                DrawRectangleLines(pos.x + dirs[d].x - 6, pos.y + dirs[d].y - 6, 12, 12, BLACK);
            }
        };
        EndDrawing();
    }
    
    UnloadTexture(background);
    CloseWindow();
    return 0;
}