#pragma once

#include "Maze.h"
#include "fssimplewindow.h"

class Entity {
public:
    Entity();

    int curRow;
    int curCol;

    clock_t startTime;
    clock_t endTime;

    int key = FSKEY_NULL;  // just to start it up

    bool isStarted = false;
    bool hasReached = false;

    bool manage(Maze& aMaze);
    void move(Maze& aMaze, int direction);
    void paint();
    bool reset(Maze& aMaze);  // places entity at aMaze start
    bool reachedGoal(Maze& aMaze);
};
