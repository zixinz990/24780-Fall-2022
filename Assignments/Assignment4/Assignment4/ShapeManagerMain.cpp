// ShapeManagerMain.cpp : main() for Shape2D program
//   by Nestor Gomez, to be updated as needed
//   


#include <iostream>

#include "fssimplewindow.h"
#include "ShapeManager.h"

using namespace std;

int main()
{
	// set up shape manager
	ShapeManager theManager(800, 600);

	theManager.showMenu();

	while (theManager.manage()) {
		// actually display graphics
		FsSwapBuffers();

		// prepare for next loop
		FsSleep(25);
	}

	return 0;
}
