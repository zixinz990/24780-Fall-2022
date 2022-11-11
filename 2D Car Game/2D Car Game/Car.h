/*
Zixin Zhang
Carnegie Mellon University
Engineering Computation, 24-780-B
Individual Project
2D Car Game
Oct. 29, 2022
*/
#pragma once
#include "Params.h"
#include "Road.h"
#include "fssimplewindow.h"
#include "DrawingUtilNG.h"
#include <vector>

using namespace std;

class Car {
protected:
	Vector startPos = { DEFAULT_START_X, DEFAULT_START_Y };
	Vector curPos = { DEFAULT_START_X, DEFAULT_START_Y }; // current position in the window frame
	Vector curVel = { DEFAULT_VEL_X, DEFAULT_VEL_Y }; // current velocity in the world frame
	Vector curAcc; // current acceleration in the window frame

	//vector<float[5]> traj; // the trajectory of one action, each element is an array: {posX, posY, velX, velY, t}
	
	int curState;

	bool move = false;

public:
	Color color = { 50, 50, 255 }; // current color
	int length = 80;
	int width = 50;
	vector<float> xTraj;
	bool haveTraj = false;

	void reset();

	void update();

	void setCurPos(float posX, float posY) { curPos = { posX, posY }; }
	void setCurVel(float velX, float velY) { curVel = { velX, velY }; }
	void setColor(float R, float G, float B) { color = { R, G, B }; }

	Vector getCurPos() { return curPos; }
	Vector getCurVel() { return curVel; }

	// generate the trajectory of moving the car to the target position in T ms
	bool genMoveTraj(Vector tgtPos, float T);

	// generate the trajectory of accelerating the car with acc for T ms
	bool accel(Vector acc);

	// paint a car
	void paint();
};