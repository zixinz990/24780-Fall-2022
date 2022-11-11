#include "Road.h"

void Road::paint() {
	using namespace DrawingUtilNG;

	// paint road surface
	glColor3ub(roadSurfColor.R, roadSurfColor.G, roadSurfColor.B);
	drawRectangle(0, 0, WINDOW_X_SIZE, WINDOW_Y_SIZE, true);

	// paint lane marking
	glColor3ub(laneMarkingColor.R, laneMarkingColor.G, laneMarkingColor.B);

	drawRectangle(laneMarkingX - laneMarkingWidth / 2, laneMarkingY_1 - laneMarkingLength / 2,
		laneMarkingWidth, laneMarkingLength, true);
	drawRectangle(laneMarkingX - laneMarkingWidth / 2, laneMarkingY_2 - laneMarkingLength / 2,
		laneMarkingWidth, laneMarkingLength, true);
	drawRectangle(laneMarkingX - laneMarkingWidth / 2, laneMarkingY_3 - laneMarkingLength / 2,
		laneMarkingWidth, laneMarkingLength, true);
	drawRectangle(laneMarkingX - laneMarkingWidth / 2, laneMarkingY_4 - laneMarkingLength / 2,
		laneMarkingWidth, laneMarkingLength, true);
	drawRectangle(laneMarkingX - laneMarkingWidth / 2, laneMarkingY_5 - laneMarkingLength / 2,
		laneMarkingWidth, laneMarkingLength, true);

	laneMarkingY_1 = laneMarkingY_1 + playerCarVelY * DT;
	laneMarkingY_2 = laneMarkingY_2 + playerCarVelY * DT;
	laneMarkingY_3 = laneMarkingY_3 + playerCarVelY * DT;
	laneMarkingY_4 = laneMarkingY_4 + playerCarVelY * DT;
	laneMarkingY_5 = laneMarkingY_5 + playerCarVelY * DT;

	if (laneMarkingY_1 - laneMarkingLength / 2 >= WINDOW_Y_SIZE) {
		laneMarkingY_1 = -(laneMarkingLength + 40) / 2;
	}
	if (laneMarkingY_2 - laneMarkingLength / 2 >= WINDOW_Y_SIZE) {
		laneMarkingY_2 = -(laneMarkingLength + 40) / 2;
	}
	if (laneMarkingY_3 - laneMarkingLength / 2 >= WINDOW_Y_SIZE) {
		laneMarkingY_3 = -(laneMarkingLength + 40) / 2;
	}
	if (laneMarkingY_4 - laneMarkingLength / 2 >= WINDOW_Y_SIZE) {
		laneMarkingY_4 = -(laneMarkingLength + 40) / 2;
	}
	if (laneMarkingY_5 - laneMarkingLength / 2 >= WINDOW_Y_SIZE) {
		laneMarkingY_5 = -(laneMarkingLength + 40) / 2;
	}
}
