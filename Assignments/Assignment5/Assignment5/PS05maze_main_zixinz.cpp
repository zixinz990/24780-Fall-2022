#include <fstream>
#include <iostream>
#include <sstream>

#include "Entity.h"
#include "Maze.h"

using namespace std;

int main() {
    Maze aMaze;
    Entity aEntity;

    aMaze.showMenu();
    std::cout << aMaze << std::endl;

    FsOpenWindow(16, 16, 800, 800, 1, "Maze");
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    FsPollDevice();
    while (aEntity.manage(aMaze)) {
        FsSwapBuffers();
        FsSleep(25);
    }
}
