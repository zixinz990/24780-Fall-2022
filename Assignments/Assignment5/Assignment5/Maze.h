#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "fssimplewindow.h"

struct Cell {
    int row;  // begin from 1
    int col;  // begin from 1
};

class Maze {
public:
    int startRow = 0;  // begin from 1
    int startCol = 0;  // begin from 1

    int endRow = 0;  // begin from 1
    int endCol = 0;  // begin from 1

    int mapBegin;  // number of columns of the map

    std::vector<std::vector<int>> map;

    int squareSize = 20;  // the size of each square is 20x20

    Maze();

    void showMenu();
    void readMapFile(std::ifstream& input);
    void paintQuad(int x1, int x2, int y1, int y2, int R, int G, int B);
    void paint();

    bool isNavigable(int row, int col);
    Cell getStart();
    Cell getEnd();

    friend std::ostream& operator<<(std::ostream& os, const Maze& aMaze);
};
