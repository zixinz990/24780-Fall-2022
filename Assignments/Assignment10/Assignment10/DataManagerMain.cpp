#include <iostream>
#include <fstream>
#include "fssimplewindow.h"
#include "DataManager.h"

using namespace std;

int main()
{
	// set up curve manager
	DataManager theManager(900, 600);

	theManager.showMenu();

	while (theManager.manage()) {
		// actually display graphics
		FsSwapBuffers();

		// prepare for next loop
		FsSleep(25);
	}

	return 0;
}
