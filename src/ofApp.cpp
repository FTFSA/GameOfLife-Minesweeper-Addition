#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetWindowTitle("Robert's Game of Life - MineSweeper Addition - Don't Let Cells Age to 500!");
    gridWidth = ofGetWidth() / cellSize;
    gridHeight = ofGetHeight() / cellSize;

    // Initialize grids
    currentGrid.assign(gridWidth, vector<int>(gridHeight, -1)); // -1 = dead
    nextGrid.assign(gridWidth, vector<int>(gridHeight, -1));

    simulate = false;
    gameOver = false;
    ofSetFrameRate(15);

    // Initialize counters
    generationCount = 0;
    liveCellCount = 0;
    deadCellCount = 0;
    cellsDiedThisGeneration = 0;
    averageCellAge = 0;
    maxCellAge = 0;
}

//--------------------------------------------------------------
void ofApp::update() {
    if (!simulate || gameOver) return;

    // Reset counters
    liveCellCount = 0;
    cellsDiedThisGeneration = 0;
    int totalAge = 0;
    maxCellAge = 0;

    // Calculate next generation
    for (int x = 0; x < gridWidth; x++) {
        for (int y = 0; y < gridHeight; y++) {

            // Count neighbors
            int neighbors = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;

                    int nx = x + i;
                    int ny = y + j;

                    if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight) {
                        if (currentGrid[nx][ny] != -1) neighbors++;
                    }
                }
            }

            // Apply rules
            if (currentGrid[x][y] != -1) { // Alive
                if (neighbors == 2 || neighbors == 3) {
                    nextGrid[x][y] = currentGrid[x][y] + 1; // Increment age
                    totalAge += nextGrid[x][y];
                    if (nextGrid[x][y] > maxCellAge) maxCellAge = nextGrid[x][y];
                }
                else {
                    nextGrid[x][y] = -1; // Die
                    cellsDiedThisGeneration++;
                }
            }
            else { // Dead
                if (neighbors == 3) {
                    nextGrid[x][y] = 0; // Born
                    totalAge += nextGrid[x][y];
                }
                else {
                    nextGrid[x][y] = -1; // Stay dead
                }
            }

            // Update live cell count
            if (nextGrid[x][y] != -1) liveCellCount++;
        }
    }

    swap(currentGrid, nextGrid);
    generationCount++;
    deadCellCount += cellsDiedThisGeneration;

    // Calculate average age
    averageCellAge = (liveCellCount > 0) ? (static_cast<float>(totalAge) / liveCellCount) : 0;

    // Game Over check
    if (maxCellAge >= 500) {
        gameOver = true;
        simulate = false;
        spellGameOver();
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);

    // Draw cells
    for (int x = 0; x < gridWidth; x++) {
        for (int y = 0; y < gridHeight; y++) {
            if (currentGrid[x][y] != -1) {
                float ageFactor = ofMap(currentGrid[x][y], 0, 500, 0, 1, true);
                float noiseValue = ofNoise(x * 0.1, y * 0.1, ofGetFrameNum() * 0.05);

                ofColor cellColor;
                cellColor.setHsb(
                    fmod(120 + (ageFactor * 120) + (noiseValue * 40), 255),
                    200 - (ageFactor * 50),
                    255 * (0.8 + (noiseValue * 0.2)),
                    255
                );

                ofSetColor(cellColor);
                ofDrawRectangle(x * cellSize, y * cellSize, cellSize - 1, cellSize - 1);
            }
        }
    }

    // Draw grid lines
    ofSetColor(40);
    for (int x = 0; x < gridWidth; x++) {
        ofDrawLine(x * cellSize, 0, x * cellSize, ofGetHeight());
    }
    for (int y = 0; y < gridHeight; y++) {
        ofDrawLine(0, y * cellSize, ofGetWidth(), y * cellSize);
    }

    // Draw UI
    ofSetColor(255);
    ofDrawBitmapString("Space: Start/Stop  |  C: Clear  |  R: Random  |  Click: Toggle", 10, 20);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 10, ofGetHeight() - 10);
    ofDrawBitmapString("Generation: " + ofToString(generationCount), 10, 40);
    ofDrawBitmapString("Live Cells: " + ofToString(liveCellCount), 10, 60);
    ofDrawBitmapString("Total Dead Cells: " + ofToString(deadCellCount), 10, 80);
    ofDrawBitmapString("Average Cell Age: " + ofToString(averageCellAge), 10, 100);
    ofDrawBitmapString("Max Cell Age: " + ofToString(maxCellAge), 10, 120);

    // Game Over text
    if (gameOver) {
        ofSetColor(255, 0, 0);
        ofDrawBitmapString("GAME OVER! Max cell age reached 500.", 10, 140);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
    case ' ':
        simulate = !simulate;
        break;
    case 'r':
        // Randomize grid
        for (auto& col : currentGrid) {
            for (auto& cell : col) {
                cell = (ofRandom(1.0) > 0.85) ? 0 : -1;
            }
        }
        generationCount = 0;
        liveCellCount = 0;
        deadCellCount = 0;
        cellsDiedThisGeneration = 0;
        averageCellAge = 0;
        maxCellAge = 0;
        gameOver = false;
        break;
    case 'c':
        // Clear grid
        for (auto& col : currentGrid) {
            fill(col.begin(), col.end(), -1);
        }
        generationCount = 0;
        liveCellCount = 0;
        deadCellCount = 0;
        cellsDiedThisGeneration = 0;
        averageCellAge = 0;
        maxCellAge = 0;
        gameOver = false;
        break;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    int gridX = x / cellSize;
    int gridY = y / cellSize;

    if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) {
        currentGrid[gridX][gridY] = (currentGrid[gridX][gridY] == -1) ? 0 : -1;
    }
}

//--------------------------------------------------------------
void ofApp::spellGameOver() {
    vector<string> gameOverText = { "GAME OVER" };
    int startX = (gridWidth - gameOverText[0].length()) / 2;
    int startY = (gridHeight - gameOverText.size()) / 2;

    // Clear grid
    for (auto& col : currentGrid) {
        fill(col.begin(), col.end(), -1);
    }

    // Spell "GAME OVER"
    for (size_t i = 0; i < gameOverText.size(); i++) {
        for (size_t j = 0; j < gameOverText[i].length(); j++) {
            if (gameOverText[i][j] != ' ') {
                int x = startX + j;
                int y = startY + i;
                if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
                    currentGrid[x][y] = 0;
                }
            }
        }
    }
}