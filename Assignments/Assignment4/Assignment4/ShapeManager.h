#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "Shape2D.h"

class ShapeManager {
private: 
	std::vector<Shape2D> theShapes;
	int currShape = -1;

	int winWidth = 800, winHeight = 600;
	int panX = 0, panY = winHeight;
	float scale = 6.f;

	std::string fileName;
	std::ifstream inFile;
	char userChoice = 'N';
	int key = FSKEY_NULL; // just to start it up

	bool inEditMode = false;
	bool showThePoints = false;

public:
	//ShapeManager() {};

	ShapeManager(int width, int height);

	bool manage();

	void showMenu();

private:
	// resets pan and zoom so that all shapes fit on the window
	void resetView();
	void resetView(Point2D lowerB, Point2D upperB);

	// if manager is in edit mode, this paints some indication on the graphics
	// screen that this is the case. Otherwise does nothing
	void paintEditIndicator();

	Point2D getScreenCoords(Point2D worldCoords);

	Point2D getWorldCoords(Point2D screenCoords);
};