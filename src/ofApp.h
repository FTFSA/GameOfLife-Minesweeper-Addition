#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void mousePressed(int x, int y, int button);

    // Grid variables
    int gridWidth, gridHeight;
    int cellSize = 10; // Size of each cell in pixels
    vector<vector<int>> currentGrid; // -1 = dead, >=0 = alive (age)
    vector<vector<int>> nextGrid;

    // Simulation control
    bool simulate;
    bool gameOver;

    // Counters and statistics
    int generationCount;
    int liveCellCount;
    int deadCellCount;
    int cellsDiedThisGeneration;
    float averageCellAge;
    int maxCellAge;

    // Game Over function
    void spellGameOver();
};