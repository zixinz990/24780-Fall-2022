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
#include "fssimplewindow.h"
#include "DrawingUtilNG.h"
#include "time.h"
#include "Car.h"
#include <iostream>

class Road {
protected:
	Color laneMarkingColor = { 255, 170, 60 };
	Color roadSurfColor = { 70, 70, 70};

	float laneMarkingWidth = 20;
	float laneMarkingLength = 160;
	float laneMarkingX = 300;

	float laneMarkingY_1 = laneMarkingLength / 2;
	float laneMarkingY_2 = laneMarkingY_1 + laneMarkingLength + 40;
	float laneMarkingY_3 = laneMarkingY_2 + laneMarkingLength + 40;
	float laneMarkingY_4 = laneMarkingY_3 + laneMarkingLength + 40;
	float laneMarkingY_5 = laneMarkingY_4 + laneMarkingLength + 40;
	
public:
	 float playerCarVelY = DEFAULT_VEL_Y;

	void paint();
};
