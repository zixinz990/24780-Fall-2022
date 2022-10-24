#include "Maze.h"

Maze::Maze() {}

void Maze::showMenu() {
    std::cout << "Please enter the name of the Maze map you want to play: " << std::endl;

    std::string fileName;
    std::cin >> fileName;
    std::ifstream inFile;
    inFile.open(fileName);
    readMapFile(inFile);

    std::cout << "\n\n";
    std::cout << "Use these keys on the screen:" << std::endl;
    std::cout << " Arws : move the cell" << std::endl;
    std::cout << "    P : play the game" << std::endl;
    std::cout << "    R : reset the game" << std::endl;
    std::cout << "  ESC : exit program" << std::endl;
    std::cout << std::endl;
}

void Maze::readMapFile(std::ifstream& input) {
    std::string wholeLineString;  // used to read file one whole line at a time
    std::stringstream
        lineStream;     // used to easily get numbers from part of line
    int colonLocation;  // used to store location of colon in line
    std::vector<int> lineVector;
    int mapElement;
    while (!input.eof()) {
        lineStream.clear();  // just in case
        lineVector.clear();
        getline(input, wholeLineString);  // read the whole line into a string
        // if there is a colon, set up lineStream to start just after colon
        // also, remember colonLocation, just in case
        if ((colonLocation = wholeLineString.find(":")) != std::string::npos) {
            lineStream.str(wholeLineString.substr(colonLocation + 1));
        }
        else {
            colonLocation = -1;
            lineStream.str(wholeLineString);
        }

        // check for keywords in the line
        if (wholeLineString.find("Start:") !=
            std::string::npos) {  // find() returns npos when not found
            lineStream >> startRow >>
                startCol;  // put values into class variables
        }
        else if (wholeLineString.find("End:") != std::string::npos) {
            lineStream >> endRow >> endCol;
        }
        else if (wholeLineString.find("Map Begin:") != std::string::npos) {
            lineStream >> mapBegin;
        }
        else if (wholeLineString.find("Map End:") != std::string::npos) {
            break;
        }
        else if (std::isdigit(lineStream.str()[0])) {
            while (lineStream >> mapElement) {
                lineVector.push_back(mapElement);
            }
            map.push_back(lineVector);
        }
    }
}

void Maze::paintQuad(int x1, int x2, int y1, int y2, int R, int G, int B) {
    glColor3ub(R, G, B);
    glBegin(GL_QUADS);
    glVertex2i(x1, y1);
    glVertex2i(x2, y1);
    glVertex2i(x2, y2);
    glVertex2i(x1, y2);
    glEnd();
}

void Maze::paint() {
    // solve blink issue
    paintQuad(0, 800, map.size() * squareSize, 800, 200, 200, 200);
    paintQuad(mapBegin * squareSize, 800, 0, map.size() * squareSize, 255, 255,
        255);

    int curRow = 1;  // begin from 1
    int curCol = 1;  // begin from 1
    for (auto row : map) {
        curCol = 1;
        for (int element : row) {
            if (element == 1) {  // paint obstacles, black
                paintQuad((curCol - 1) * squareSize, curCol * squareSize,
                    (curRow - 1) * squareSize, curRow * squareSize, 0, 0,
                    0);
            }
            else {
                if (curRow == startRow &&
                    curCol == startCol) {  // paint start square, green
                    paintQuad((curCol - 1) * squareSize, curCol * squareSize,
                        (curRow - 1) * squareSize, curRow * squareSize, 0,
                        255, 0);
                }
                else if (curRow == endRow &&
                    curCol == endCol) {  // paint end square, red
                    paintQuad((curCol - 1) * squareSize, curCol * squareSize,
                        (curRow - 1) * squareSize, curRow * squareSize,
                        255, 0, 0);
                }
                else {  // paint white
                    paintQuad((curCol - 1) * squareSize, curCol * squareSize,
                        (curRow - 1) * squareSize, curRow * squareSize,
                        255, 255, 255);
                }
            }
            curCol = curCol + 1;
        }
        curRow = curRow + 1;
    }
}

bool Maze::isNavigable(int row, int col) {
    if (row > 0 && col > 0 && row <= map.size() && col <= mapBegin) {
        if (map[row - 1][col - 1] == 0) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

Cell Maze::getStart() {
    if (startRow == 0 || startCol == 0) {
        std::cout << "This map doesn't have a start point. Please load another map." << std::endl;
    }
    else {
        return { startRow, startCol };
    }
}

Cell Maze::getEnd() {
    if (endRow == 0 || endCol == 0) {
        std::cout << "This map doesn't have a start point. Please load another map." << std::endl;
    }
    else {
        return { endRow, endCol };
    }
}

std::ostream& operator<<(std::ostream& os, const Maze& aMaze) {
    os << "Start: " << aMaze.startRow << " " << aMaze.startCol << std::endl;
    os << "End: " << aMaze.endRow << " " << aMaze.endCol << std::endl;
    os << "Map Begin: " << aMaze.mapBegin << std::endl;

    int curCol = 1;
    for (auto currMap : aMaze.map) {
        for (auto element : currMap) {
            os << element << " ";
            if (curCol == aMaze.mapBegin) {
                os << std::endl;
                curCol = 1;
            }
            else {
                curCol++;
            }
        }
    }
    os << "Map End: " << std::endl;
    return os;
}
