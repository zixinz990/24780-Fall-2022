#include "Prop.h"

void Prop::paint() {
	using namespace DrawingUtilNG;
	// paint acc prop
	if (propExist == false && accEffective == false && deaccEffective == false) { // if there is no prop now, we may need to paint prop
		float choosePropRand = (float)rand() / RAND_MAX; // generate a random number between 0 and 1
		if (choosePropRand > 0.9) { // show acc prop if the rand num > 0.99
			propExist = true;
			showAccProp = true;

			// determine which lane the prop should appear
			float randNumAccX = (float)rand() / RAND_MAX;
			if (randNumAccX > 0.5) {
				//accPropX = 145;
				initAccProp(145, -20);
			}
			else {
				//accPropX = 600 - 145;
				initAccProp(600 - 145, -20);
			}
		}
		//else if (choosePropRand < 0.1) {
		//	propExist = true;
		//	showDeaccProp = true;

		//	// determine which lane the prop should appear
		//	float randNumDeAccX = (float)rand() / RAND_MAX;
		//	if (randNumDeAccX > 0.5) {
		//		//deaccPropX = 145;
		//		initDeaccProp(145, -20);
		//	}
		//	else {
		//		//deaccPropX = 600 - 145;
		//		initDeaccProp(600 - 145, -20);
		//	}
		//}
	}

	// paint acc prop
	
	if (showAccProp == true && accEffective == false && deaccEffective == false) {
		glColor3ub(0, 255, 0);
		drawRectangle(accPropX - 10, accPropY, 20, 20, true);
		accPropY = accPropY + playerCar->getCurVel().y * DT;
	}
	if (accPropY >= WINDOW_Y_SIZE + 10) {
		//accPropY = -20;
		showAccProp = false;
		resetProp();
	}

	// paint deacc prop
	if (showDeaccProp == true && accEffective == false && deaccEffective == false) {
		glColor3ub(255, 0, 0);
		drawRectangle(deaccPropX - 10, deaccPropY, 20, 20, true);
		deaccPropY = deaccPropY + playerCar->getCurVel().y * DT;
	}
	if (deaccPropY >= WINDOW_Y_SIZE + 10) {
		//deaccPropY = -20;
		showDeaccProp = false;
		resetProp();
	}
}

bool Prop::hitAccProp() {
	if (abs(playerCar->getCurPos().y - accPropY) < 10
		&& abs(playerCar->getCurPos().x - accPropX) < 10) {
		accEffective = true;
		resetProp();
		std::cout << "hit acc" << std::endl;
		return true;
	}
	else {
		return false;
	}
}

bool Prop::hitDeaccProp() {
	if (abs(playerCar->getCurPos().y - deaccPropY) < 20
		&& abs(playerCar->getCurPos().x - deaccPropX) < 20) {
		deaccEffective = true;
		resetProp();
		std::cout << "hit deacc" << std::endl;
		return true;
	}
	else {
		return false;
	}
}