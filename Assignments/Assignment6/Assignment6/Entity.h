/*
  ===============================
 || XXX   XXXX        XX    XXXX||
 ||   X X XXXX XXXX X    XX XXXX||  Nestor Gomez
 ||XX X XsXXXX XXXX XXXXXXX XXXX||  Carnegie Mellon University
 ||XX   X XXX    XX XXXX   XXXXX||  Eng. Computation, 24-780-B
 ||XX XXX  XX X XXX XXXX XXXXXXX||  Prob Set 5
 ||XX XXXX XXXX XXX XXXX      XX||  Due Tues. Oct. 11, 2022
 ||XX XXXX X    XXX XXXX XXXX XX||
 ||XX XXXX X XXXXXX XXXX X      ||
 ||XX XXXXXX XXXXXX XXXX XXXXXX ||
 || X  XXXXX       XXXXX XXXXXX ||  Maze navigation using keyboard to
 || XX X     XX XX  XXXX X    X ||  create simple interactive game.
 || XX X XXX XX XXX XXX XXeXX X ||  
 || XX X   X XX X X XXX XXXXX XX||	The Entity is the object that
 ||    XXX X XX X X XXX       XX||	moves around the maze.
 ||XXX     X    X   XXXXXXXXXXXX||
  ===============================

*/
#pragma once
#include <queue>
#include <iostream>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "DrawingUtilNG.h"
#include "Maze.h"
#include "Maze.h"

//class Maze; // needed as holding spot for any class that includes Entity.h

class Entity {
protected:
	enum TorchShape { EVERYTHING, SQUARE, CIRCLE, OVAL, CLOUD};
	int locRow, locCol; // current location of entity
	int celebrateCounter; // used to control celebration animation
	bool visited[100][100];

	int torchSize = 5;
	TorchShape theTorchShape = EVERYTHING;

	Cell parent[100][100];

	Maze* myMaze = nullptr;

public:
	void setMaze(Maze& aMaze) { myMaze = &aMaze; };

	int getRow() { return locRow; }
	int getCol() { return locCol; }

	void reset();

	// returns true if move was successful
	bool move(int direction);

	// for fun, this draws where the entity has been using visited array
	void paintMotionTrack();

	// draw the entity at current location
	void paint();

	// check if the current location is the end of the maze
	bool reachedGoal();

	// do a little animation while the entity is at the end of the maze
	void celebrateGoal();

	// playing around with hiding the map and only showing what can be
	// seen using a "torch"
	int getTorchSize() { return torchSize; }
	void disableTorch() { theTorchShape = EVERYTHING; }
	void increaseTorch() { torchSize++; }
	void decreaseTorch() { if (torchSize > 1) torchSize--; }
	void toggleTorchShape() {
		theTorchShape = (TorchShape)((theTorchShape + 1) % 3);
	}

	void findShortestPath();

};
