#include <iostream>
#include <fstream>
#include "ShapeManager.h"

using namespace std;

void ShapeManager::showMenu()
{
	// Some of these options need to be programmed by you for PS04

	cout << "\n\n";
	cout << "Use these keys on the screen:" << endl;
	cout << " Arws : pan in all directions" << endl;
	cout << "  +/- : zoom in and out" << endl;
	cout << "    R : Reset the view so all shapes fit" << endl;		// PS04

	cout << endl;
	cout << "    L : Load shape from file" << endl;
	cout << "    C : Clear all shapes" << endl;
	cout << endl;

	cout << "    E : toggle Edit mode" << endl;								    // PS04
	cout << "  \< \> : cycle through shapes to select current shape" << endl;	// PS04 (use FSKEY_COMMA and FSKEY_DOT)
	cout << "    P : toggle showing of Points" << endl;						    // PS04
	cout << "    H : change Hue angle of shape by +10 deg" << endl;			    // PS04
	cout << "    Z : Zoom into just the current shape" << endl;			        // PS04
	cout << "    S : Save the current shape" << endl;			                // PS04
	cout << endl;

	cout << "  ESC : exit program" << endl;
}

// TODO
void ShapeManager::resetView()
{
	// find lower bound and upper bound for all shapes
	if (theShapes.size() > 0) {
		Point2D overallLowerB = theShapes.at(0).getLowerBound();
		Point2D overallUpperB = theShapes.at(0).getUpperBound();

		Point2D curLowerB = theShapes.at(0).getLowerBound();
		Point2D curUpperB = theShapes.at(0).getUpperBound();

		for (int i = 1; i < theShapes.size(); i++) {
			curLowerB = theShapes[i].getLowerBound();
			curUpperB = theShapes[i].getUpperBound();
			if (curLowerB.x < overallLowerB.x) {
				overallLowerB.x = curLowerB.x;
			}
			if (curLowerB.y < overallLowerB.y) {
				overallLowerB.y = curLowerB.y;
			}
			if (curUpperB.x > overallUpperB.x) {
				overallUpperB.x = curUpperB.x;
			}
			if (curUpperB.y > overallUpperB.y) {
				overallUpperB.y = curUpperB.y;
			}
		}

		// call other function
		resetView(overallLowerB, overallUpperB);
	}
}

void ShapeManager::resetView(Point2D lowerB, Point2D upperB)
{
	int extraBorder = 24; // allows for some space around all shape(s)

	// Determine needed scale to completely fill in the screen.
	// We do it for each direction and then choose smallest as overall?
	// new scale, leaving blank space for other direction?

	float horzScale = (winWidth - extraBorder) / (upperB.x - lowerB.x);
	float vertScale = (winHeight - extraBorder) / (upperB.y - lowerB.y);
	scale = min(horzScale, vertScale);

	// To adjust panX and panY, we need to put center of all shapes
	// at the center of screen. Thus, we first pan to center of screen
	// and then pan from center of shapes (average of shape bounds)
	// to world 0,0 ? converted to pixels. Note minus for x (moving left)
	// and plus for y (moving down) since we are using pixel dimensions

	panX = winWidth / 2. - (upperB.x + lowerB.x) / 2. * scale;
	panY = winHeight / 2. + (upperB.y + lowerB.y) / 2. * scale;
}

void ShapeManager::paintEditIndicator()
{
}

Point2D ShapeManager::getScreenCoords(Point2D worldCoords)
{
	float screenX = worldCoords.x * scale + panX;
	float screenY = worldCoords.y * -scale + panY;
	return { screenX, screenY };
}

Point2D ShapeManager::getWorldCoords(Point2D screenCoords)
{
	float worldX = (screenCoords.x - panX) / scale;
	float worldY = (screenCoords.y - panY) / -scale;
	return { worldX, worldY };
}

ShapeManager::ShapeManager(int width, int height)
{
	// for testing, just load 3 shapes (comment out when done)
	ifstream inFile;
	inFile.open("spur01.shape");
	theShapes.push_back(Shape2D(inFile));
	inFile.close();
	inFile.open("sample01.shape");
	theShapes.push_back(Shape2D(inFile));
	inFile.close();
	inFile.open("rectangle01.shape");
	theShapes.push_back(Shape2D(inFile));
	inFile.close();
	resetView();

	winWidth = width;
	winHeight = height;
	FsOpenWindow(16, 16, winWidth, winHeight, 1, "Shape Manager (NG 2022)");

	// set color and width
	glColor3b(255, 0, 0); // red
	//glLineWidth(3);

}

bool ShapeManager::manage()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	FsPollDevice();
	key = FsInkey();

	switch (key) {
	case FSKEY_L: // load a file and add to shapes
		cout << endl << "   Enter a name of shape file to load > ";
		cin >> fileName;
		if (fileName.length() > 0) {
			if (fileName.find(".shape") == string::npos)
				fileName += ".shape";

			inFile.open(fileName);
			if (inFile.is_open()) {
				theShapes.push_back(Shape2D(inFile));
				inFile.close();
				cout << "       A new shape was added from file " << fileName << endl;
				cout << "       There are now " << theShapes.size() << " shapes." << endl;
			}
			else
				cout << "       ERROR: Was not able to open file " << fileName << endl;
		}
		showMenu(); // So that it is "fresh"
		break;
	case FSKEY_C: // clear all shapes (with confirmation)
		cout << "Are you sure you want to clear all shapes (Y/N) >> ";
		cin >> userChoice;
		if (userChoice == 'Y' || userChoice == 'y') {
			theShapes.clear(); // empties all members of a vector (kills memory too)
		}
		showMenu(); // So that it is "fresh"
		break;
	case FSKEY_RIGHT: panX += 5;
		break;
	case FSKEY_LEFT: panX -= 5;
		break;
	case FSKEY_UP: panY -= 5;
		break;
	case FSKEY_DOWN: panY += 5;
		break;
	case FSKEY_PLUS: scale *= 1.05;
		break;
	case FSKEY_MINUS: scale /= 1.05;
		break;
	case FSKEY_R: resetView();
		break;
	case FSKEY_E:
		inEditMode = !inEditMode;
		if (inEditMode) {
			if (theShapes.size() == 0) {
				inEditMode = false;
				std::cout << "You can't get into edit mode because there is no shape." << std::endl;
			}
			else {
				std::cout << "You are in edit mode now." << std::endl;
			}
		}
		else {
			std::cout << "You leave edit mode now." << std::endl;
			for (auto& currShape : theShapes) {
				currShape.colorOverride = -1.0;
			}
		}
		break;
	}

	//cout << "    H : change Hue angle of shape by +10 deg" << endl;			    // PS04
	//cout << "    S : Save the current shape" << endl;			                // PS04

	if (inEditMode) {
		switch (key) {
		case FSKEY_COMMA: // cycle through shapes to select current shape
			// the initial value of currShape = -1
			if (currShape == -1) {
				currShape = 0;
				theShapes[currShape].colorOverride = 1.0;
				break;
			}
			else {
				theShapes[currShape].colorOverride = -1.0;
				currShape--;
				if (currShape < 0) {
					currShape = 0;
				}
				theShapes[currShape].colorOverride = 1.0;
				break;
			}
		case FSKEY_DOT: // cycle through shapes to select current shape
			// the initial value of currShape = -1
			if (currShape == -1) {
				currShape = 0;
				theShapes[currShape].colorOverride = 1.0;
				break;
			}
			else {
				theShapes[currShape].colorOverride = -1.0;
				currShape++;
				if (currShape > theShapes.size() - 1) {
					currShape = theShapes.size() - 1;
				}
				theShapes[currShape].colorOverride = 1.0;
				break;
			}
		case FSKEY_P: // toggle showing of Points
			if (currShape < 0 || currShape > theShapes.size() - 1) {
				currShape = 0;
			}
			theShapes[currShape].showThePoints = !theShapes[currShape].showThePoints;
		case FSKEY_H:
			theShapes[currShape].colorHue = theShapes[currShape].colorHue + 10;
			if (theShapes[currShape].colorHue > 360) {
				theShapes[currShape].colorHue = theShapes[currShape].colorHue - 360;
			}
		case FSKEY_Z: // Zoom into just the current shape
			if (currShape < 0 || currShape > theShapes.size() - 1) {
				break;
			}
			else {
				resetView(theShapes.at(currShape).getLowerBound(), theShapes.at(currShape).getUpperBound());
				break;
			}
		}
	}

	// set up axes to "math normal" (origin at lower left, y-axis going up)
	// and pan and scale
	glLoadIdentity();

	//// handle mouse input (OpenGL is still in screen coords)
	//int mouseEvent, leftButton, middleButton, rightButton;
	//int screenX, screenY;

	//mouseEvent = FsGetMouseEvent(leftButton, middleButton,
	//	rightButton, screenX, screenY);

	//if (leftButton) {
	//	// write coords on screen if left button is held down
	//	stringstream coordStream; // for displaying coordinates on screen
	//	Point2D screenPnt = { screenX, screenY };
	//	Point2D worldPnt = getWorldCoords(screenPnt);

	//	coordStream.str(""); // reset stream
	//	coordStream.precision(4);
	//	coordStream << worldPnt.x << ", " << worldPnt.y
	//		<< " (" << screenX << ", " << screenY << ")";
	//	glColor3ub(60, 230, 60);
	//	glRasterPos2i(screenX, screenY - 3); // set position 3 pix above
	//	YsGlDrawFontBitmap7x10(coordStream.str().c_str());
	//}

	glTranslatef(panX, panY, 0);
	glScalef(scale, -scale, 1);

	// draw all the shapes
	for (auto& currShape : theShapes) {
		currShape.paint(true, false, currShape.showThePoints, currShape.colorOverride);
	}

	return key != FSKEY_ESC;
}



// The following functions are for use by the few people who had trouble
// reading from a data file. It is simply a "hardwire" of the data 

/*

void loadShape_sample01(Shape2D& aShape)
{
	aShape.addPoint(-0.924, 0.383, 1000);
	aShape.addPoint(-1.000, 0.000, 1000);
	aShape.addPoint(-0.924, -0.383, 1000);
	aShape.addPoint(-0.707, -0.707, 1000);
	aShape.addPoint(-0.383, -0.924, 1000);
	aShape.addPoint(0.000, -1.000, 1000);
	aShape.addPoint(0.383, -0.924, 1000);
	aShape.addPoint(10.383, 4.076, 1000);
	aShape.addPoint(10.707, 4.293, 1000);
	aShape.addPoint(10.924, 4.617, 1000);
	aShape.addPoint(11.000, 5.000, 1000);
	aShape.addPoint(10.924, 5.383, 1000);
	aShape.addPoint(6.924, 13.383, 1000);
	aShape.addPoint(6.707, 13.707, 1000);
	aShape.addPoint(6.383, 13.924, 1000);
	aShape.addPoint(6.000, 14.000, 1000);
	aShape.addPoint(5.617, 13.924, 1000);
	aShape.addPoint(5.293, 13.707, 1000);
	aShape.addPoint(5.076, 13.383, 1000);
}

void loadShape_rectangle01(Shape2D& aShape)
{
	aShape.addPoint(2.000, 3.500, 1000);
	aShape.addPoint(4.000, 3.500, 1000);
	aShape.addPoint(4.000, 8.200, 1000);
	aShape.addPoint(2.000, 8.200, 1000);
}

void loadShape_spur01(Shape2D& aShape)
{
	aShape.addPoint(6.000, 3.000, 1000);
	aShape.addPoint(5.923, 3.780, 1000);
	aShape.addPoint(6.157, 4.722, 1000);
	aShape.addPoint(5.742, 5.500, 1000);
	aShape.addPoint(4.828, 5.828, 1000);
	aShape.addPoint(4.222, 6.326, 1000);
	aShape.addPoint(3.722, 7.157, 1000);
	aShape.addPoint(2.878, 7.414, 1000);
	aShape.addPoint(2.000, 7.000, 1000);
	aShape.addPoint(1.220, 6.923, 1000);
	aShape.addPoint(0.278, 7.157, 1000);
	aShape.addPoint(-0.500, 6.742, 1000);
	aShape.addPoint(-0.828, 5.828, 1000);
	aShape.addPoint(-1.326, 5.222, 1000);
	aShape.addPoint(-2.157, 4.722, 1000);
	aShape.addPoint(-2.414, 3.878, 1000);
	aShape.addPoint(-2.000, 3.000, 1000);
	aShape.addPoint(-1.923, 2.220, 1000);
	aShape.addPoint(-2.157, 1.278, 1000);
	aShape.addPoint(-1.742, 0.500, 1000);
	aShape.addPoint(-0.828, 0.172, 1000);
	aShape.addPoint(-0.222, -0.326, 1000);
	aShape.addPoint(0.278, -1.157, 1000);
	aShape.addPoint(1.122, -1.414, 1000);
	aShape.addPoint(2.000, -1.000, 1000);
	aShape.addPoint(2.780, -0.923, 1000);
	aShape.addPoint(3.722, -1.157, 1000);
	aShape.addPoint(4.500, -0.742, 1000);
	aShape.addPoint(4.828, 0.172, 1000);
	aShape.addPoint(5.326, 0.778, 1000);
	aShape.addPoint(6.157, 1.278, 1000);
	aShape.addPoint(6.414, 2.122, 1000);
}

void loadShape_pachiderm(Shape2D& aShape)
{
	aShape.addPoint(181.720, 71.740, 1000);
	aShape.addPoint(181.590, 71.810, 1000);
	aShape.addPoint(181.430, 71.800, 1000);
	aShape.addPoint(181.280, 71.790, 1000);
	aShape.addPoint(180.130, 71.910, 1000);
	aShape.addPoint(176.720, 72.070, 1000);
	aShape.addPoint(174.810, 72.470, 1000);
	aShape.addPoint(172.890, 72.880, 1000);
	aShape.addPoint(171.200, 73.540, 1000);
	aShape.addPoint(169.800, 74.240, 1000);
	aShape.addPoint(168.550, 74.890, 1000);
	aShape.addPoint(167.510, 75.810, 1000);
	aShape.addPoint(166.410, 76.640, 1000);
	aShape.addPoint(165.950, 80.190, 1000);
	aShape.addPoint(165.330, 83.710, 1000);
	aShape.addPoint(164.380, 87.160, 1000);
	aShape.addPoint(163.460, 90.490, 1000);
	aShape.addPoint(162.440, 93.210, 1000);
	aShape.addPoint(160.940, 96.630, 1000);
	aShape.addPoint(159.860, 99.300, 1000);
	aShape.addPoint(158.880, 101.290, 1000);
	aShape.addPoint(157.950, 103.130, 1000);
	aShape.addPoint(157.020, 104.970, 1000);
	aShape.addPoint(156.330, 106.110, 1000);
	aShape.addPoint(155.350, 107.680, 1000);
	aShape.addPoint(153.310, 111.920, 1000);
	aShape.addPoint(153.340, 111.090, 1000);
	aShape.addPoint(152.100, 112.550, 1000);
	aShape.addPoint(150.850, 114.020, 1000);
	aShape.addPoint(150.660, 115.560, 1000);
	aShape.addPoint(147.880, 116.480, 1000);
	aShape.addPoint(144.050, 118.490, 1000);
	aShape.addPoint(139.580, 117.900, 1000);
	aShape.addPoint(135.420, 118.080, 1000);
	aShape.addPoint(133.970, 118.150, 1000);
	aShape.addPoint(130.350, 119.560, 1000);
	aShape.addPoint(129.210, 119.240, 1000);
	aShape.addPoint(128.060, 119.370, 1000);
	aShape.addPoint(128.670, 119.080, 1000);
	aShape.addPoint(128.510, 118.890, 1000);
	aShape.addPoint(128.360, 118.700, 1000);
	aShape.addPoint(129.700, 118.100, 1000);
	aShape.addPoint(128.310, 118.100, 1000);
	aShape.addPoint(126.790, 118.040, 1000);
	aShape.addPoint(122.310, 118.970, 1000);
	aShape.addPoint(120.120, 118.880, 1000);
	aShape.addPoint(117.940, 118.790, 1000);
	aShape.addPoint(116.700, 118.340, 1000);
	aShape.addPoint(115.170, 117.560, 1000);
	aShape.addPoint(114.990, 117.570, 1000);
	aShape.addPoint(114.800, 117.570, 1000);
	aShape.addPoint(114.620, 117.580, 1000);
	aShape.addPoint(112.590, 117.400, 1000);
	aShape.addPoint(107.020, 117.190, 1000);
	aShape.addPoint(103.040, 116.530, 1000);
	aShape.addPoint(99.070, 115.880, 1000);
	aShape.addPoint(95.090, 114.360, 1000);
	aShape.addPoint(90.790, 113.650, 1000);
	aShape.addPoint(86.330, 112.760, 1000);
	aShape.addPoint(81.790, 112.090, 1000);
	aShape.addPoint(77.230, 112.270, 1000);
	aShape.addPoint(73.400, 112.420, 1000);
	aShape.addPoint(69.570, 113.160, 1000);
	aShape.addPoint(65.750, 112.840, 1000);
	aShape.addPoint(62.400, 112.580, 1000);
	aShape.addPoint(60.060, 111.950, 1000);
	aShape.addPoint(57.150, 110.710, 1000);
	aShape.addPoint(54.230, 109.480, 1000);
	aShape.addPoint(51.910, 108.130, 1000);
	aShape.addPoint(48.240, 105.430, 1000);
	aShape.addPoint(43.410, 102.370, 1000);
	aShape.addPoint(38.520, 99.130, 1000);
	aShape.addPoint(35.100, 94.530, 1000);
	aShape.addPoint(32.240, 91.350, 1000);
	aShape.addPoint(32.140, 89.620, 1000);
	aShape.addPoint(31.040, 86.320, 1000);
	aShape.addPoint(29.940, 83.020, 1000);
	aShape.addPoint(29.110, 78.350, 1000);
	aShape.addPoint(28.500, 74.760, 1000);
	aShape.addPoint(27.900, 71.160, 1000);
	aShape.addPoint(27.880, 68.210, 1000);
	aShape.addPoint(27.400, 64.730, 1000);
	aShape.addPoint(26.920, 61.250, 1000);
	aShape.addPoint(26.630, 57.300, 1000);
	aShape.addPoint(25.130, 53.870, 1000);
	aShape.addPoint(24.010, 49.810, 1000);
	aShape.addPoint(22.360, 45.490, 1000);
	aShape.addPoint(18.380, 44.150, 1000);
	aShape.addPoint(19.300, 43.460, 1000);
	aShape.addPoint(20.630, 43.390, 1000);
	aShape.addPoint(21.610, 43.980, 1000);
	aShape.addPoint(21.250, 42.910, 1000);
	aShape.addPoint(20.240, 42.090, 1000);
	aShape.addPoint(19.120, 41.950, 1000);
	aShape.addPoint(20.390, 41.520, 1000);
	aShape.addPoint(21.910, 42.030, 1000);
	aShape.addPoint(22.670, 43.140, 1000);
	aShape.addPoint(22.570, 42.150, 1000);
	aShape.addPoint(22.760, 41.140, 1000);
	aShape.addPoint(23.210, 40.250, 1000);
	aShape.addPoint(23.670, 41.150, 1000);
	aShape.addPoint(24.530, 45.210, 1000);
	aShape.addPoint(25.420, 48.570, 1000);
	aShape.addPoint(26.310, 51.940, 1000);
	aShape.addPoint(27.390, 55.610, 1000);
	aShape.addPoint(28.560, 60.440, 1000);
	aShape.addPoint(29.730, 65.270, 1000);
	aShape.addPoint(31.860, 76.970, 1000);
	aShape.addPoint(32.450, 77.540, 1000);
	aShape.addPoint(33.920, 81.480, 1000);
	aShape.addPoint(32.360, 67.830, 1000);
	aShape.addPoint(32.050, 63.880, 1000);
	aShape.addPoint(31.740, 59.920, 1000);
	aShape.addPoint(30.790, 56.540, 1000);
	aShape.addPoint(30.560, 53.800, 1000);
	aShape.addPoint(30.640, 51.680, 1000);
	aShape.addPoint(31.040, 49.550, 1000);
	aShape.addPoint(30.700, 47.450, 1000);
	aShape.addPoint(30.380, 45.480, 1000);
	aShape.addPoint(29.410, 43.670, 1000);
	aShape.addPoint(28.310, 42.000, 1000);
	aShape.addPoint(25.140, 37.160, 1000);
	aShape.addPoint(20.770, 33.120, 1000);
	aShape.addPoint(15.710, 30.340, 1000);
	aShape.addPoint(14.340, 29.590, 1000);
	aShape.addPoint(12.740, 28.680, 1000);
	aShape.addPoint(12.570, 27.120, 1000);
	aShape.addPoint(12.480, 26.380, 1000);
	aShape.addPoint(12.770, 25.640, 1000);
	aShape.addPoint(13.090, 24.970, 1000);
	aShape.addPoint(13.420, 24.200, 1000);
	aShape.addPoint(13.840, 23.390, 1000);
	aShape.addPoint(14.510, 22.530, 1000);
	aShape.addPoint(15.180, 21.670, 1000);
	aShape.addPoint(16.130, 20.540, 1000);
	aShape.addPoint(17.120, 19.800, 1000);
	aShape.addPoint(18.110, 19.050, 1000);
	aShape.addPoint(18.990, 18.580, 1000);
	aShape.addPoint(20.460, 18.080, 1000);
	aShape.addPoint(21.930, 17.580, 1000);
	aShape.addPoint(25.100, 16.210, 1000);
	aShape.addPoint(25.930, 16.790, 1000);
	aShape.addPoint(26.750, 17.370, 1000);
	aShape.addPoint(25.380, 20.110, 1000);
	aShape.addPoint(25.580, 21.540, 1000);
	aShape.addPoint(25.790, 22.960, 1000);
	aShape.addPoint(25.400, 23.480, 1000);
	aShape.addPoint(27.160, 25.340, 1000);
	aShape.addPoint(28.920, 27.200, 1000);
	aShape.addPoint(33.070, 30.300, 1000);
	aShape.addPoint(36.150, 32.680, 1000);
	aShape.addPoint(39.540, 35.310, 1000);
	aShape.addPoint(42.210, 38.870, 1000);
	aShape.addPoint(45.140, 42.030, 1000);
	aShape.addPoint(45.490, 40.720, 1000);
	aShape.addPoint(46.030, 39.450, 1000);
	aShape.addPoint(46.250, 38.110, 1000);
	aShape.addPoint(46.630, 35.900, 1000);
	aShape.addPoint(46.640, 33.560, 1000);
	aShape.addPoint(46.630, 31.320, 1000);
	aShape.addPoint(46.630, 28.940, 1000);
	aShape.addPoint(46.530, 26.160, 1000);
	aShape.addPoint(46.240, 23.850, 1000);
	aShape.addPoint(45.950, 21.530, 1000);
	aShape.addPoint(42.610, 18.530, 1000);
	aShape.addPoint(44.870, 17.410, 1000);
	aShape.addPoint(59.810, 17.090, 1000);
	aShape.addPoint(60.510, 17.080, 1000);
	aShape.addPoint(61.420, 17.250, 1000);
	aShape.addPoint(61.490, 17.940, 1000);
	aShape.addPoint(61.530, 18.260, 1000);
	aShape.addPoint(61.350, 18.550, 1000);
	aShape.addPoint(61.180, 18.820, 1000);
	aShape.addPoint(60.540, 20.460, 1000);
	aShape.addPoint(57.230, 22.480, 1000);
	aShape.addPoint(57.650, 27.760, 1000);
	aShape.addPoint(57.460, 31.440, 1000);
	aShape.addPoint(56.330, 37.740, 1000);
	aShape.addPoint(57.340, 41.530, 1000);
	aShape.addPoint(58.350, 45.320, 1000);
	aShape.addPoint(62.270, 48.690, 1000);
	aShape.addPoint(63.690, 50.490, 1000);
	aShape.addPoint(65.120, 52.290, 1000);
	aShape.addPoint(65.020, 51.930, 1000);
	aShape.addPoint(65.900, 52.340, 1000);
	aShape.addPoint(66.700, 52.710, 1000);
	aShape.addPoint(67.590, 52.810, 1000);
	aShape.addPoint(68.460, 52.910, 1000);
	aShape.addPoint(75.370, 53.700, 1000);
	aShape.addPoint(82.210, 55.200, 1000);
	aShape.addPoint(88.820, 57.380, 1000);
	aShape.addPoint(90.540, 57.950, 1000);
	aShape.addPoint(92.520, 58.540, 1000);
	aShape.addPoint(94.060, 57.590, 1000);
	aShape.addPoint(95.260, 56.850, 1000);
	aShape.addPoint(95.800, 55.390, 1000);
	aShape.addPoint(96.070, 54.000, 1000);
	aShape.addPoint(96.350, 51.530, 1000);
	aShape.addPoint(96.220, 46.190, 1000);
	aShape.addPoint(95.730, 42.760, 1000);
	aShape.addPoint(95.240, 39.320, 1000);
	aShape.addPoint(93.840, 36.570, 1000);
	aShape.addPoint(93.120, 33.370, 1000);
	aShape.addPoint(92.580, 30.080, 1000);
	aShape.addPoint(92.490, 26.700, 1000);
	aShape.addPoint(91.400, 23.550, 1000);
	aShape.addPoint(90.480, 20.890, 1000);
	aShape.addPoint(88.850, 18.480, 1000);
	aShape.addPoint(86.720, 16.640, 1000);
	aShape.addPoint(91.070, 14.940, 1000);
	aShape.addPoint(95.920, 15.510, 1000);
	aShape.addPoint(100.550, 16.090, 1000);
	aShape.addPoint(101.550, 16.220, 1000);
	aShape.addPoint(102.840, 16.680, 1000);
	aShape.addPoint(102.770, 17.690, 1000);
	aShape.addPoint(102.740, 18.110, 1000);
	aShape.addPoint(102.460, 18.470, 1000);
	aShape.addPoint(102.250, 18.840, 1000);
	aShape.addPoint(101.440, 20.310, 1000);
	aShape.addPoint(101.820, 22.140, 1000);
	aShape.addPoint(102.330, 23.750, 1000);
	aShape.addPoint(103.070, 26.340, 1000);
	aShape.addPoint(105.290, 30.970, 1000);
	aShape.addPoint(106.720, 34.400, 1000);
	aShape.addPoint(108.150, 37.830, 1000);
	aShape.addPoint(109.740, 40.860, 1000);
	aShape.addPoint(110.890, 44.320, 1000);
	aShape.addPoint(111.930, 47.900, 1000);
	aShape.addPoint(112.410, 51.640, 1000);
	aShape.addPoint(113.620, 55.160, 1000);
	aShape.addPoint(114.270, 57.030, 1000);
	aShape.addPoint(115.130, 58.850, 1000);
	aShape.addPoint(116.050, 60.650, 1000);
	aShape.addPoint(116.120, 60.340, 1000);
	aShape.addPoint(116.200, 60.030, 1000);
	aShape.addPoint(116.300, 59.730, 1000);
	aShape.addPoint(116.770, 58.330, 1000);
	aShape.addPoint(117.600, 57.080, 1000);
	aShape.addPoint(118.360, 55.810, 1000);
	aShape.addPoint(120.490, 52.200, 1000);
	aShape.addPoint(122.010, 48.270, 1000);
	aShape.addPoint(123.530, 44.360, 1000);
	aShape.addPoint(127.380, 34.400, 1000);
	aShape.addPoint(131.730, 23.200, 1000);
	aShape.addPoint(132.210, 21.960, 1000);
	aShape.addPoint(132.690, 20.690, 1000);
	aShape.addPoint(132.710, 19.360, 1000);
	aShape.addPoint(132.720, 18.030, 1000);
	aShape.addPoint(132.150, 16.610, 1000);
	aShape.addPoint(130.990, 15.980, 1000);
	aShape.addPoint(141.510, 15.800, 1000);
	aShape.addPoint(142.110, 15.790, 1000);
	aShape.addPoint(142.780, 15.810, 1000);
	aShape.addPoint(143.170, 16.260, 1000);
	aShape.addPoint(143.730, 16.880, 1000);
	aShape.addPoint(143.510, 15.680, 1000);
	aShape.addPoint(143.020, 18.610, 1000);
	aShape.addPoint(142.530, 21.550, 1000);
	aShape.addPoint(140.780, 28.830, 1000);
	aShape.addPoint(140.220, 33.880, 1000);
	aShape.addPoint(139.650, 38.920, 1000);
	aShape.addPoint(140.410, 44.570, 1000);
	aShape.addPoint(139.610, 48.880, 1000);
	aShape.addPoint(138.810, 53.180, 1000);
	aShape.addPoint(137.810, 56.390, 1000);
	aShape.addPoint(136.640, 60.010, 1000);
	aShape.addPoint(135.280, 63.560, 1000);
	aShape.addPoint(133.230, 66.880, 1000);
	aShape.addPoint(132.580, 70.620, 1000);
	aShape.addPoint(132.320, 72.080, 1000);
	aShape.addPoint(132.300, 73.600, 1000);
	aShape.addPoint(132.650, 75.040, 1000);
	aShape.addPoint(133.980, 73.960, 1000);
	aShape.addPoint(135.170, 72.710, 1000);
	aShape.addPoint(136.180, 71.320, 1000);
	aShape.addPoint(136.940, 70.870, 1000);
	aShape.addPoint(137.030, 71.820, 1000);
	aShape.addPoint(137.220, 72.320, 1000);
	aShape.addPoint(137.420, 72.830, 1000);
	aShape.addPoint(137.350, 73.430, 1000);
	aShape.addPoint(137.360, 74.350, 1000);
	aShape.addPoint(137.360, 75.270, 1000);
	aShape.addPoint(136.690, 76.800, 1000);
	aShape.addPoint(137.260, 77.850, 1000);
	aShape.addPoint(137.510, 78.310, 1000);
	aShape.addPoint(137.920, 78.650, 1000);
	aShape.addPoint(138.180, 79.100, 1000);
	aShape.addPoint(138.500, 79.640, 1000);
	aShape.addPoint(138.570, 80.220, 1000);
	aShape.addPoint(138.470, 80.800, 1000);
	aShape.addPoint(139.000, 81.160, 1000);
	aShape.addPoint(140.380, 81.320, 1000);
	aShape.addPoint(141.360, 81.260, 1000);
	aShape.addPoint(142.340, 81.200, 1000);
	aShape.addPoint(143.490, 80.840, 1000);
	aShape.addPoint(144.340, 80.440, 1000);
	aShape.addPoint(145.190, 80.040, 1000);
	aShape.addPoint(145.790, 79.450, 1000);
	aShape.addPoint(146.440, 78.860, 1000);
	aShape.addPoint(147.080, 78.270, 1000);
	aShape.addPoint(147.760, 77.460, 1000);
	aShape.addPoint(148.210, 76.910, 1000);
	aShape.addPoint(148.490, 76.430, 1000);
	aShape.addPoint(148.740, 75.910, 1000);
	aShape.addPoint(149.160, 75.550, 1000);
	aShape.addPoint(150.350, 74.540, 1000);
	aShape.addPoint(152.460, 75.170, 1000);
	aShape.addPoint(153.460, 73.980, 1000);
	aShape.addPoint(153.750, 73.640, 1000);
	aShape.addPoint(153.890, 73.200, 1000);
	aShape.addPoint(154.020, 72.760, 1000);
	aShape.addPoint(155.340, 68.120, 1000);
	aShape.addPoint(155.280, 63.160, 1000);
	aShape.addPoint(156.600, 58.520, 1000);
	aShape.addPoint(157.600, 54.800, 1000);
	aShape.addPoint(158.820, 52.930, 1000);
	aShape.addPoint(160.010, 50.460, 1000);
	aShape.addPoint(161.190, 47.990, 1000);
	aShape.addPoint(162.600, 46.110, 1000);
	aShape.addPoint(163.710, 43.670, 1000);
	aShape.addPoint(164.820, 41.240, 1000);
	aShape.addPoint(165.870, 38.480, 1000);
	aShape.addPoint(166.680, 35.860, 1000);
	aShape.addPoint(167.490, 33.240, 1000);
	aShape.addPoint(168.550, 30.310, 1000);
	aShape.addPoint(168.600, 27.970, 1000);
	aShape.addPoint(169.010, 25.720, 1000);
	aShape.addPoint(169.020, 22.810, 1000);
	aShape.addPoint(166.960, 21.820, 1000);
	aShape.addPoint(166.050, 21.380, 1000);
	aShape.addPoint(164.940, 21.460, 1000);
	aShape.addPoint(164.080, 20.920, 1000);
	aShape.addPoint(163.230, 20.380, 1000);
	aShape.addPoint(163.200, 18.660, 1000);
	aShape.addPoint(164.210, 18.670, 1000);
	aShape.addPoint(163.250, 17.080, 1000);
	aShape.addPoint(163.940, 16.950, 1000);
	aShape.addPoint(166.740, 17.100, 1000);
	aShape.addPoint(168.350, 17.920, 1000);
	aShape.addPoint(169.950, 18.740, 1000);
	aShape.addPoint(171.870, 20.600, 1000);
	aShape.addPoint(172.880, 22.000, 1000);
	aShape.addPoint(174.750, 23.550, 1000);
	aShape.addPoint(174.250, 24.450, 1000);
	aShape.addPoint(174.440, 26.340, 1000);
	aShape.addPoint(174.640, 28.240, 1000);
	aShape.addPoint(174.650, 30.340, 1000);
	aShape.addPoint(174.030, 33.370, 1000);
	aShape.addPoint(173.280, 37.180, 1000);
	aShape.addPoint(171.790, 40.810, 1000);
	aShape.addPoint(170.700, 44.540, 1000);
	aShape.addPoint(169.850, 47.710, 1000);
	aShape.addPoint(169.460, 48.980, 1000);
	aShape.addPoint(168.930, 52.420, 1000);
	aShape.addPoint(168.400, 55.860, 1000);
	aShape.addPoint(166.700, 62.860, 1000);
	aShape.addPoint(167.520, 65.170, 1000);
	aShape.addPoint(169.720, 65.030, 1000);
	aShape.addPoint(171.920, 65.240, 1000);
	aShape.addPoint(173.850, 66.270, 1000);
	aShape.addPoint(174.310, 66.510, 1000);
	aShape.addPoint(174.750, 67.200, 1000);
	aShape.addPoint(174.300, 67.460, 1000);
	aShape.addPoint(174.170, 67.530, 1000);
	aShape.addPoint(174.010, 67.520, 1000);
	aShape.addPoint(173.860, 67.510, 1000);
	aShape.addPoint(171.650, 67.350, 1000);
	aShape.addPoint(169.420, 67.570, 1000);
	aShape.addPoint(167.280, 68.100, 1000);
	aShape.addPoint(167.190, 69.140, 1000);
	aShape.addPoint(167.100, 70.170, 1000);
	aShape.addPoint(167.000, 71.210, 1000);
	aShape.addPoint(168.650, 70.470, 1000);
	aShape.addPoint(170.480, 70.040, 1000);
	aShape.addPoint(172.260, 69.760, 1000);
	aShape.addPoint(175.290, 69.280, 1000);
	aShape.addPoint(178.550, 69.100, 1000);
	aShape.addPoint(181.260, 70.550, 1000);
	aShape.addPoint(181.720, 70.790, 1000);
	aShape.addPoint(182.170, 71.480, 1000);
}

void loadShape(const string& filename, Shape2D& aShape) {
	if (filename == "sample01.shape")
		loadShape_sample01(aShape);
	else if (filename == "rectangle01.shape")
		loadShape_rectangle01(aShape);
	else if (filename == "spur01.shape")
		loadShape_spur01(aShape);
	else if (filename == "pachiderm.shape")
		loadShape_pachiderm(aShape);

}

*/
