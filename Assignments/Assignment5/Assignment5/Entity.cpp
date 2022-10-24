#include "Entity.h"

Entity::Entity() {}

bool Entity::manage(Maze& aMaze) {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    FsPollDevice();
    key = FsInkey();

    move(aMaze, key);

    if (reachedGoal(aMaze) && hasReached == false) {
        hasReached = true;
        endTime = clock();
        std::cout << "You reached the goal!" << std::endl;
        std::cout << "You spent "
            << (float)((endTime - startTime) / CLOCKS_PER_SEC)
            << " seconds." << std::endl;
        std::cout << "Press R to play again." << std::endl;
    }

    switch (key) {
    case FSKEY_P:
        if (isStarted) {
            break;
        }
        else {
            reset(aMaze);
            isStarted = true;
            startTime = clock();
            std::cout << "Game starts!" << std::endl;
            break;
        }
    case FSKEY_R:
        if (isStarted) {
            reset(aMaze);
            hasReached = false;
            startTime = clock();
            std::cout << "Game resets!" << std::endl;
        }
        else {
            std::cout << "You haven't started the game!" << std::endl;
        }
        break;
    }

    aMaze.paint();
    paint();

    return key != FSKEY_ESC;
}

void Entity::move(Maze& aMaze, int direction) {
    switch (direction) {
    case FSKEY_RIGHT:
        if (isStarted) {
            if (aMaze.isNavigable(curRow, curCol + 1)) {
                curCol = curCol + 1;
            }
            else {
                std::cout << "You hit the obstacle!" << std::endl;
            }
            break;
        }
        else {
            std::cout << "You haven't started the game!" << std::endl;
            break;
        }
    case FSKEY_LEFT:
        if (isStarted) {
            if (aMaze.isNavigable(curRow, curCol - 1)) {
                curCol = curCol - 1;
            }
            else {
                std::cout << "You hit the obstacle!" << std::endl;
            }
            break;
        }
        else {
            std::cout << "You haven't started the game!" << std::endl;
            break;
        }
    case FSKEY_UP:
        if (isStarted) {
            if (aMaze.isNavigable(curRow - 1, curCol)) {
                curRow = curRow - 1;
            }
            else {
                std::cout << "You hit the obstacle!" << std::endl;
            }
            break;
        }
        else {
            std::cout << "You haven't started the game!" << std::endl;
            break;
        }
    case FSKEY_DOWN:
        if (isStarted) {
            if (aMaze.isNavigable(curRow + 1, curCol)) {
                curRow = curRow + 1;
            }
            else {
                std::cout << "You hit the obstacle!" << std::endl;
            }
            break;
        }
        else {
            std::cout << "You haven't started the game!" << std::endl;
            break;
        }
    }
}

void Entity::paint() {
    glColor3ub(0, 0, 255);  // blue
    glBegin(GL_QUADS);
    glVertex2i(curCol * 20, curRow * 20);
    glVertex2i(curCol * 20, (curRow - 1) * 20);
    glVertex2i((curCol - 1) * 20, curRow * 20);
    glVertex2i((curCol - 1) * 20, (curRow - 1) * 20);
    glEnd();
}

bool Entity::reset(Maze& aMaze) {
    curRow = aMaze.getStart().row;
    curCol = aMaze.getStart().col;
    return true;
}

bool Entity::reachedGoal(Maze& aMaze) {
    if (curRow == aMaze.getEnd().row && curCol == aMaze.getEnd().col) {
        return true;
    }
    else {
        return false;
    }
}
