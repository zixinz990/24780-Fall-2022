#include <iostream>
#include <fstream>
#include <sstream>
#include "DrawingUtilNG.h"
#include "GraphicFont.h"
#include "StringPlus.h"
#include "ysglfontdata.h"
#include "DataManager.h"

using namespace std;

DataManager::DataManager(int width, int height)
{
	// for testing, just load a curve (comment out when done)
	//ifstream inFile("loop2.curve");
	//theTrack.readFile(inFile);
	//inFile.close();

	winWidth = width;
	winHeight = height;

	FsOpenWindow(16, 16, winWidth, winHeight, 1, "Data Manager");

	// set color and width
	glColor3b(255, 0, 0);  // red
	//glLineWidth(3);

		// load sounds and start player
	//if (YSOK != insideSound.LoadWav("cash_register.wav"))
	//	cout << "   ERROR: Unable to load cash_register.wav " << endl;
	//if (YSOK != outsideSound.LoadWav("buzzer.wav"))
	//	cout << "   ERROR: Unable to load cash_register.wav " << endl;
	//theSoundPlayer.Start();

	ButtonCollection* myButtons = new ButtonCollection; // put this AFTER FsOpenWindow()
	GraphicFont* buttonFont = new ComicSansFont;
	buttonFont->setColorRGB(0, 0, 0); // black

	addButtonsVert(buttonFont, winWidth - 108, 100);

	resetView();
}

void DataManager::showMenu()
{
	cout << "\n\n";
	cout << "Use these keys on the screen:" << endl;
	cout << " Arws : pan in all directions (also Ctrl+LeftButtonDrag)" << endl;
	cout << "  +/- : zoom in and out (also Shft+LeftButtonDrag)" << endl;
	cout << "    Z : Zoom the view so whole curve fits" << endl;		// PS04
	cout << "    H : change Hue angle of shape by +10 deg" << endl;			// PS04
	cout << endl;

	cout << "    L : Load a dataset from file" << endl;
	cout << "    S : Save the dataset" << endl;			// PS04
	cout << "    E : toggle Edit mode" << endl;								// PS04
	cout << "      Left Mouse : move points" << endl;
	cout << "      D : Delete higlighted point" << endl;
	cout << "      A : Add a point just back of higlighted point" << endl;			// PS04
	cout << endl;

	cout << "SPACE : start/pause simulation" << endl;

	cout << "  ESC : exit program" << endl;
}

void DataManager::resetView()
{
	if (fitting.getLowerBound().x == -INFINITY)
		resetView({ 0, 0 }, { 20,20 });
	else
		resetView(fitting.getLowerBound(), fitting.getUpperBound());
}

void DataManager::resetView(Point2D lowerB, Point2D upperB)
{
	int extraBorder = 24; // allows for some space around all shape(s)

	// Determine needed scale to completely fill in the screen.
	// We do it for each direction and then choose smallest as overall 
	// new scale, leaving blank space for other direction 

	float horzScale = (winWidth - 100 - extraBorder) / (upperB.x - lowerB.x);
	float vertScale = (winHeight - extraBorder) / (upperB.y - lowerB.y);
	scaleX = horzScale;
	scaleY = vertScale;

	// To adjust panX and panY, we need to put center of all shapes
	// at the center of screen. Thus, we first pan to center of screen
	// and then pan from center of shapes (average of shape bounds)
	// to world 0,0  , converted to pixels. Note minus for x (moving left)
	// and plus for y (moving down) since we are using pixel dimensions

	panX = winWidth / 2. - (upperB.x + lowerB.x) / 2. * scaleX;
	panY = winHeight / 2. + (upperB.y + lowerB.y) / 2. * scaleY;
}

void DataManager::paintEditIndicator()
{
	bool useAwful = false;

	if (!useAwful) {
		// draw yellow square all around, with the word "EDIT" at top right and
		// bottom left corners

		// box around (big yellow rectangle with slightly smaller white rectangle)
		glColor3ub(255, 255, 0); // yellow
		int frameThickness = 5;
		DrawingUtilNG::drawRectangle(frameThickness, frameThickness,
			winWidth - 2 * frameThickness - 1, winHeight - 2 * frameThickness - 1, true,
			winWidth, winHeight);

		// little boxes for EDIT label
		glColor3ub(255, 255, 0); // yellow
		int boxW = 40, boxH = 18;
		DrawingUtilNG::drawRectangle(winWidth - boxW, 0, boxW, boxH, true);
		DrawingUtilNG::drawRectangle(0, winHeight - boxH, boxW, boxH, true);

		// label "EDIT"
		glColor3ub(0, 0, 0); // black
		glRasterPos2i(winWidth - boxW + 3, boxH - 3);  // top right
		YsGlDrawFontBitmap8x12("EDIT");
		glRasterPos2i(3, winHeight - 3);  // bottom left
		YsGlDrawFontBitmap8x12("EDIT");

		// little box for instructions on adding and deleting points
		glColor3ub(255, 255, 0); // yellow
		DrawingUtilNG::drawRectangle(winWidth, winHeight, -4 * boxW, -2 * boxH, true);
		glColor3ub(0, 0, 0); // black
		glRasterPos2i(winWidth - 3.7 * boxW + 3, winHeight - boxH - 3);
		YsGlDrawFontBitmap8x12("A : Add point");
		glRasterPos2i(winWidth - 3.7 * boxW + 3, winHeight - 3);
		YsGlDrawFontBitmap8x12("D : Delete point");

	}
	else {
		// awful edit indicator (but simple to code :-)
		glColor3ub(0, 0, 0); // black
		glRasterPos2i(0, 30);  // top right
		YsGlDrawFontBitmap20x28("YOU ARE NOW IN EDIT MODE");
	}
}

Point2D DataManager::getScreenCoords(Point2D worldCoords)
{
	float screenX = worldCoords.x * scaleX + panX;
	float screenY = worldCoords.y * -scaleY + panY;
	return { screenX, screenY };
}

Point2D DataManager::getWorldCoords(Point2D screenCoords)
{
	float worldX = (screenCoords.x - panX) / scaleX;
	float worldY = (screenCoords.y - panY) / -scaleY;
	return { worldX, worldY };
}

void DataManager::manageMouse()
{
	// handle mouse input (OpenGL is still in screen coords)
	stringstream coordStream;     // for displaying coordinates on screen
	FsPollDevice();
	mouseEvent = FsGetMouseEvent(leftButton, middleButton,
		rightButton, screenX, screenY);

	Point2D worldPnt = getWorldCoords({ screenX * 1.f, screenY * 1.f });

	if (leftButton) { // write coords on screen if left button is held down
		coordStream.str("");  // reset stream
		coordStream.precision(4);
		coordStream << worldPnt.x << ", " << worldPnt.y
			<< " (" << screenX << ", " << screenY << ")";
		glColor3ub(60, 230, 60);
		glRasterPos2i(screenX, screenY - 3);  // set position 3 pix above
		YsGlDrawFontBitmap7x10(coordStream.str().c_str());
	}

	if (inEditMode) {
		float scale = min(scaleX, scaleY);
		float hoverDistance = 3.f / scale; // 3 pixels

		// figure out if there's a point near mouse location
		if (!vertexIsMoving) { // avoid changing currVertex while moving a vertex
			currVertex = fitting.getIndex(worldPnt, hoverDistance);
			//if (currVertex > 0)
			//	cout << "Hovered over Pnt " << currVertex << endl;
		}
		if (mouseEvent == FSMOUSEEVENT_LBUTTONUP) {
			vertexIsMoving = false;
		}
		else if (leftButton && mouseEvent == FSMOUSEEVENT_MOVE && currVertex > 0) {
			vertexIsMoving = true;
			fitting.movePoint(worldPnt, currVertex);
			fitting.fit();
		}
	}

}

void DataManager::addButtonsVert(GraphicFont* aFont, int xLoc, int wid)
{
	int hei = 30;
	int spacing = 10;

	int currY = 30;
	theButtons.add(xLoc, currY, wid, hei, FSKEY_L, "Load", aFont,
		"Load a dataset from a file");

	currY += hei + spacing;
	theButtons.add(xLoc, currY, wid, hei, FSKEY_S, "Save", aFont,
		"Save the dataset to a file");

	currY += hei + spacing;
	theButtons.add(xLoc, currY, wid, hei, FSKEY_H, "Hue", aFont,
		"Cycle through several colors of curve");

	//currY += hei + spacing;
	//theButtons.add(xLoc, currY, wid, hei, FSKEY_Q, "Sound", aFont,
	//	"Toggle sound feedback on/off");

	currY += hei + spacing;
	theButtons.add(xLoc, currY, wid, hei, FSKEY_Z, "View All", aFont,
		"Adjust view to fit the whole curve");

	currY += hei + spacing * 3;
	theButtons.add(xLoc, currY, wid, hei, FSKEY_E, "Edit", aFont,
		"Toggle Edit Mode");
}

bool DataManager::manage()
{
	ofstream outFile;
	int buttonKey;

	ComicSansFont comicsans;
	comicsans.setColorHSV(0, 1, 1);
	std::string curveStr;

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// reset all transformations
	glLoadIdentity();

	FsPollDevice();
	key = FsInkey();

	// handle mouse input (remember that OpenGL is still in screen coords)
	manageMouse();

	// check if a button was clicked
	if (key == FSKEY_NULL && mouseEvent == FSMOUSEEVENT_LBUTTONDOWN) {
		buttonKey = theButtons.checkClick(screenX, screenY);

		if (buttonKey != FSKEY_NULL)
			key = buttonKey;  // pretend the user pressed a key 
	}

	switch (key) {
	case FSKEY_L: // load a curve file
		fileName = DrawingUtilNG::getStringFromScreen("Enter name of file to load.",
			"Press ENTER when done, ESC to cancel.");
		if (fileName.length() > 0) {

			if (fileName.find(".txt") == string::npos)
				fileName += ".txt";

			inFile.open(fileName);
			if (inFile.is_open()) {
				fitting.readFile(inFile);
				inFile.close();
				fitting.fit();
				cout << "       New dataset was loaded from file " << fileName << endl;

				// change zoom to show newly loaded data
				resetView();
			}
			else
				cout << "       ERROR: Was not able to open file " << fileName << endl;
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
	case FSKEY_PLUS: scaleX *= 1.05; scaleY *= 1.05;
		break;
	case FSKEY_MINUS: scaleX /= 1.05; scaleY /= 1.05;
		break;

		// new stuff for PS04
	case FSKEY_Z:   // reset Zoom to fit all shapes
		resetView();
		break;
	case FSKEY_E:
		inEditMode = !inEditMode;
		break;
	case FSKEY_S:
		fileName = DrawingUtilNG::getStringFromScreen("Enter name of file to save.",
			"Press ENTER when done, ESC to cancel.");
		if (fileName.length() > 0) {

			if (fileName.find(".txt") == string::npos)
				fileName += ".txt";

			outFile.open(fileName);
			if (outFile.is_open()) {
				outFile << fitting;
				outFile.close();
				cout << "Just saved " << fileName << " to current folder." << endl;
			}
			else {
				cout << "Cannot open file " << fileName << " for output." << endl;
			}
			showMenu();
		}
		break;
	case FSKEY_H:
		fitting.changeColor(10.f);
		break;
	}

	if (inEditMode) {  // also for PS04
		paintEditIndicator();
		switch (key) {
		case FSKEY_A:   // add a point for spline control
			if (currVertex > 0) { // there is a highlighted point
				fitting.addPoint(currVertex, 0.5);
			}
			break;
		case FSKEY_D:   // delete a point
			if (currVertex > 0) { // there is a highlighted point
				fitting.removePoint(currVertex);
			}
			break;
		case FSKEY_P:  // toggle showing points for current shape
			showThePoints = !showThePoints;
			break;
		}
	}

	curveStr = "F = " + std::to_string(fitting.coef.a1) + " * x + " + std::to_string(fitting.coef.a2) + " * x^2";

	comicsans.setColorHSV(0, 0, 0);
	comicsans.drawText(curveStr, 20, 30, 0.2);

	// draw edit mode indicator
	if (inEditMode)
		paintEditIndicator();

	// set up axes to "math normal" (origin at lower left, y-axis going up)
	// and pan and scale
	glTranslatef(panX, panY, 0);
	glScalef(scaleX, -scaleY, 1);

	//fitedCurve.paint(false, false, true);
	fitting.paint(false, useFillMode, showThePoints || inEditMode);

	// highlight point that could be selected
	if (inEditMode && currVertex > 0) {
		glColor3ub(215, 215, 0);
		fitting.paintPoint(currVertex, 1.);
	}

	glLoadIdentity();
	theButtons.paint();
	theButtons.checkHover(screenX, screenY); // remove hover feedback for better performance 

	return key != FSKEY_ESC;

}