/*  Nestor Gomez
	24-780-B, Carnegie Mellon
	Oct. 11, 2022

	An object-oriented program to test maze navigation.
*/
#pragma warning(disable:4996)
#include <iostream>
#include <string>
#include <fstream>
#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "Maze.h"
#include "Entity.h"

using namespace std;

void loadFile(Maze& theMaze)
{
	bool continueAsking = true;
	string fileToOpen;

	while (continueAsking) {
		cout << "Enter file name to load >> ";
		cin >> fileToOpen;

		if (fileToOpen.length() > 0) {
			// so I don't have to type so much during testing
			if (fileToOpen == "a")
				fileToOpen = "mazeA_10x10.map";
			else if (fileToOpen == "b")
				fileToOpen = "mazeB_25x25.map";
			else if (fileToOpen == "c")
				fileToOpen = "mazeC_40x20.map";

			ifstream inFile(fileToOpen);
			if (inFile.is_open()) {
				theMaze.readFile(inFile);
				continueAsking = false;
				inFile.close();
			}
			else {
				cout << "Could not open file. ";
			}
		}
		else // blank entry for filename so cancel load file
			continueAsking = false;
	}
}

void showMenu()
{
	cout << endl;
	cout << "Instructions: " << endl;
	cout << "    Arrows to move entity." << endl;
	cout << "    'L' to Load a maze file" << endl;
	cout << "    'R' to Reset entity to start" << endl;
	cout << "    'H' to Hide/unHide map" << endl;
	cout << "       '+/-' to increase/decrease torch size" << endl;
	cout << "    'Q' to toggle sound Quiet/loud" << endl;
	cout << "    'W' to write maze to file" << endl;
	//cout << "    'C' to create a new maze" << endl;
	cout << endl;
	//	cout << "    'G' use BFS to find shortest path from entity to end." << endl;
	//	cout << "    'A' use A* to find shortest path from entity to end." << endl;
	//	cout << "    'K' to set cost of traversing an obstacle." << endl;
	//	cout << endl;
	cout << "    Maze manipulation" << endl;
	cout << "       'T' to Turn maze: 90 degrees CW, Shift+T for CCW" << endl;
	cout << "       'F' to Flip maze left-right" << endl;
	cout << "       'U' to turn maze Upsidedown" << endl;
	cout << "       'M' to mirror maze about its diagonal" << endl;
	cout << endl;
	cout << "    Letter on a square to set start/end (if navigable)" << endl;
	cout << "       'S' for start" << endl;
	cout << "       'E' for end" << endl;
	cout << "       Click mouse on cell to toggle navigable/obstacle" << endl;
	cout << endl;
}

void saveFile(Maze& theMaze)
{
	string fileToSave;

	cout << "Enter file name to save >> ";
	cin >> fileToSave;
	if (fileToSave.length() > 0) {
		ofstream outFile(fileToSave);
		if (outFile.is_open()) {
			outFile << theMaze;
			outFile.close();
		}
		else {
			cout << "Could not save file. ";
		}
	}
}

int main(void)
{
	Maze theMaze;
	Entity theEntity;
	bool soundIsOn = true;
	bool findPathOn = false;
	int key = FSKEY_NULL;
	int mouseEvent, leftButton, middleButton, rightButton, locX, locY;

	FsOpenWindow(16, 16, (Maze::MAX_MAP_SIZE - 1) * Maze::BLOCK_SIZE,
		(Maze::MAX_MAP_SIZE - 1) * Maze::BLOCK_SIZE, 1,
		"Maze Game (NG 2022)");

	// audio feedback on allowed/disallowed moves
	YsSoundPlayer theSoundPlayer;
	YsSoundPlayer::SoundData yesMoveSound;
	YsSoundPlayer::SoundData cantMoveSound;
	if (YSOK != yesMoveSound.LoadWav("click_x.wav"))
		cout << "Can't find yesMoveSound data" << endl;
	if (YSOK != cantMoveSound.LoadWav("gulp_x.wav"))
		cout << "Can't find cantMoveSound data" << endl;
	theSoundPlayer.Start();

	showMenu();
	theEntity.setMaze(theMaze);
	theEntity.reset();
	FsPollDevice();
	while (FSKEY_ESC != key)
	{
		FsPollDevice();
		key = FsInkey();
		mouseEvent = FsGetMouseEvent(leftButton, middleButton, rightButton, locX, locY);

		// take care of window resizing
		int wid, hei;
		FsGetWindowSize(wid, hei);
		glViewport(0, 0, wid, hei);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, (float)wid - 1, (float)hei - 1, 0, -1, 1);

		// draw all the stuff
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		theMaze.paint(theEntity.reachedGoal());

		theEntity.paintMotionTrack();
		theEntity.paint();

		if (theEntity.reachedGoal()) {
			theEntity.disableTorch(); // show everything
			theEntity.celebrateGoal();
		}
		FsSwapBuffers();

		// take care of user interactions
		switch (key) {
		case FSKEY_UP:
		case FSKEY_DOWN:
		case FSKEY_LEFT:
		case FSKEY_RIGHT:
			if (theEntity.move(key)) {
				if (soundIsOn) theSoundPlayer.PlayOneShot(yesMoveSound);
				if (findPathOn) theEntity.findShortestPath();
			}
			else
				if (soundIsOn) theSoundPlayer.PlayOneShot(cantMoveSound);
			break;
		case FSKEY_Q:
			soundIsOn = !soundIsOn;
			break;
		case FSKEY_L:
			loadFile(theMaze);
			theEntity.reset();
			// theEntity.findShortestPath();
			showMenu();
			break;
		case FSKEY_R: theEntity.reset();
			break;
		case FSKEY_H:
			theEntity.toggleTorchShape();
			break;
		case FSKEY_PLUS:
			theEntity.increaseTorch();
			break;
		case FSKEY_MINUS:
			theEntity.decreaseTorch();
			break;
		case FSKEY_W:
			saveFile(theMaze);
			showMenu();
			break;
		case FSKEY_T:
			theMaze.quarterTurn();
			if (FsGetKeyState(FSKEY_SHIFT)) {
				theMaze.quarterTurn();
				theMaze.quarterTurn();
			}
			theEntity.reset();
			break;
		case FSKEY_F:
			theMaze.mirrorLeftRight();
			theEntity.reset();
			break;
		case FSKEY_U:
			theMaze.mirrorUpDown();
			theEntity.reset();
			break;
		case FSKEY_M:
			theMaze.mirrorOnDiagonal();
			theEntity.reset();
			break;
		case FSKEY_S:
		case FSKEY_E:
			theMaze.changeMapState(locX, locY, key);
			theEntity.reset();
			break;
		case FSKEY_G:
			if (findPathOn == false) {
				theEntity.findShortestPath();
				findPathOn = true;
			}
			break;
		}

		FsSleep(20);
	}
}