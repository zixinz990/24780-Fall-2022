#include "Entity.h"

void Entity::reset()
{
	// mark everything as not visited
	for (int i = 1; i < Maze::MAX_MAP_SIZE; i++)
		for (int j = 1; j < Maze::MAX_MAP_SIZE; j++)
			visited[i][j] = false;

	// put entity back at start loc
	locRow = myMaze->getStart().row;
	locCol = myMaze->getStart().col;

	// reset celebrateCounter
	celebrateCounter = 0;
}

bool Entity::move(int direction)
{
	int testCol = locCol, testRow = locRow;

	switch (direction) {
	case FSKEY_UP: testRow--;
		break;
	case FSKEY_DOWN: testRow++;
		break;
	case FSKEY_LEFT: testCol--;
		break;
	case FSKEY_RIGHT: testCol++;
		break;
	}
	if (myMaze->isNavigable(testRow, testCol)) {
		// record location where entity used to be
		visited[locRow][locCol] = true;
		// change location of entity
		locCol = testCol;
		locRow = testRow;
		// findShortestPath();
		return true;
	}
	else
		return false;
}

void Entity::paintMotionTrack()
{
	int xCenter;
	int yCenter;

	glColor3ub(0, 0, 255);

	// draw motion track
	for (int i = 1; i < Maze::MAX_MAP_SIZE; i++)
		for (int j = 1; j < Maze::MAX_MAP_SIZE; j++)
			if (visited[i][j]) {
				xCenter = (j - 0.5) * Maze::BLOCK_SIZE;
				yCenter = (i - 0.5) * Maze::BLOCK_SIZE;
				DrawingUtilNG::drawRectangle(xCenter - 2, yCenter - 2, 4, 4, false);
			}
}

void Entity::paint()
{
	int xCenter = (locCol - 0.5) * Maze::BLOCK_SIZE;
	int yCenter = (locRow - 0.5) * Maze::BLOCK_SIZE;

	glColor3ub(0, 0, 255);  // blue

	// legs
	glBegin(GL_LINES);
	glVertex2i(xCenter - 8, yCenter - 5);
	glVertex2i(xCenter + 8, yCenter + 5);

	glVertex2i(xCenter + 8, yCenter - 5);
	glVertex2i(xCenter - 8, yCenter + 5);

	glVertex2i(xCenter + 8, yCenter - 0);
	glVertex2i(xCenter - 8, yCenter + 0);
	glEnd();

	// head
	glBegin(GL_LINE_LOOP);
	glVertex2i(xCenter, yCenter - 2);
	glVertex2i(xCenter - 3, yCenter - 6);
	glVertex2i(xCenter - 2, yCenter - 7);
	glVertex2i(xCenter + 2, yCenter - 7);
	glVertex2i(xCenter + 3, yCenter - 6);
	glEnd();

	// draw the shortest path using parent array, starting at end
	// and walking back to beginning
	Cell _cell = parent[myMaze->getEnd().row][myMaze->getEnd().col];
	if (_cell.row > 0 && _cell.col > 0) {
		while (_cell.row != locRow || _cell.col != locCol) {
			// std::cout << "row: " << _cell.row << ", col: " << _cell.col << std::endl;
			int xCenter;
			int yCenter;

			glColor3ub(0, 0, 0);
			xCenter = (_cell.col - 0.5) * Maze::BLOCK_SIZE;
			yCenter = (_cell.row - 0.5) * Maze::BLOCK_SIZE;
			DrawingUtilNG::drawRectangle(xCenter - 2, yCenter - 2, 4, 4, true);

			_cell = parent[_cell.row][_cell.col];			
		}
	}

	// map hiding
	if (theTorchShape != EVERYTHING) {

		// Draw a big grey blob, with a hole in the middle

		using namespace DrawingUtilNG;
		int wid, hei;
		FsGetWindowSize(wid, hei);  // get window size
		glColor3ub(200, 200, 200);

		if (theTorchShape == CIRCLE)
			drawCircle((locCol - 0.5) * Maze::BLOCK_SIZE, (locRow - 0.5) * Maze::BLOCK_SIZE,
				torchSize * Maze::BLOCK_SIZE, torchSize * Maze::BLOCK_SIZE,
				true, 2. * (wid + hei), 2. * (wid + hei));

		else if (theTorchShape == SQUARE)
			drawRectangle((locCol - torchSize / 2. - 0.5) * Maze::BLOCK_SIZE, (locRow - torchSize / 2. - 0.5) * Maze::BLOCK_SIZE,
				torchSize * Maze::BLOCK_SIZE, torchSize * Maze::BLOCK_SIZE,
				true, 2. * (wid + hei), 2. * (wid + hei));

	}
}

bool Entity::reachedGoal()
{
	return (myMaze->getEnd().row != -1
		&& locRow == myMaze->getEnd().row && locCol == myMaze->getEnd().col);
}

void Entity::celebrateGoal()
{
	using namespace DrawingUtilNG;

	int flashRate = 6;
	float radius = (celebrateCounter / flashRate + 2) * Maze::BLOCK_SIZE / 2;
	int xCenter = (locCol - 0.5) * Maze::BLOCK_SIZE; // the 0.5 centers on the block
	int yCenter = (locRow - 0.5) * Maze::BLOCK_SIZE;

	glColor3ub(255, 255, 0);
	glLineWidth(4);
	drawCircle(xCenter, yCenter, radius, radius * 1.2, false);
	drawCircle(xCenter, yCenter, radius * 1.5, radius * 1.8, false);
	glLineWidth(1);

	// increase counter and reset if needed
	if (++celebrateCounter > flashRate * 5)
		celebrateCounter = 0;
}

void Entity::findShortestPath()
{
	using namespace std;

	bool visited[Maze::MAX_MAP_SIZE][Maze::MAX_MAP_SIZE];
	// Cell startCell = myMaze->getStart();
	Cell endCell = myMaze->getEnd();
	Cell currCell;
	queue<Cell> processQueue;

	// for cycling through adjacent cells
	int rowAdjust[] = { -1, 0, 1, 0 };
	int colAdjust[] = { 0, 1, 0, -1 };
	int testRow, testCol;

	for (int i = 1; i < Maze::MAX_MAP_SIZE; i++) {
		for (int j = 1; j < Maze::MAX_MAP_SIZE; j++) {
			visited[i][j] = false;
			parent[i][j] = { -1,-1 };
		}
	}

	// mark the root/start vertex as visited
	visited[locRow][locCol] = true;
	// visited[startCell.row][startCell.col] = true;

	// add the root vertex to the process queue
	processQueue.push({ locRow, locCol });
	// processQueue.push({ startCell.row, startCell.col });
	bool foundTheEnd = false;

	// while the end vertex is yet to be discovered and there are vertices to process
	while (processQueue.size() > 0 && !foundTheEnd) {
		// get the current vertex at the head of the queue (removing it from the queue)
		currCell = processQueue.front();
		processQueue.pop(); // remove from queue

		// if current vertex is the target vertexd, we are done
		if (currCell.row == endCell.row && currCell.col == endCell.col) {
			foundTheEnd = true;
		}
		else {
			// else, for each of the adjacent vertices that have not been visited:
			// these will help me check the four neighbor cells
			// then we are able loop over all possible neighbors
			for (int i = 0; i < 4; i++) {
				testRow = currCell.row + rowAdjust[i];
				testCol = currCell.col + colAdjust[i];

				if (!visited[testRow][testCol] && myMaze->isNavigable(testRow, testCol)) {
					// set its parrent to current vertex
					parent[testRow][testCol] = currCell;
					// mark as visited
					visited[testRow][testCol] = true;
					// add it to the queue
					processQueue.push({ testRow, testCol });
				}
			}
		}
	}

	if (parent[endCell.row][endCell.col].row == -1) {
		cout << "Could not find a path!" << endl;
	}
}