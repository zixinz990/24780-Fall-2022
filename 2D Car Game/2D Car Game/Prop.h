#pragma once
#include "Params.h"
#include "fssimplewindow.h"
#include "DrawingUtilNG.h"
#include "Car.h"
#include <iostream>

class Prop {
protected:
	float width = 20.0;
	float length = 20.0;

	// acc prop
	float accPropX = 145;
	float accPropY = -20;
	bool showAccProp = false;

	// deacc prop
	float deaccPropX = 145;
	float deaccPropY = -20;
	bool showDeaccProp = false;

	bool propExist = false;

	bool accEffective = false;
	bool deaccEffective = false;

	Car* playerCar = nullptr;

public:
	void paint();

	void setPlayer(Car& aCar) { playerCar = &aCar; }

	void initAccProp(float x, float y) {
		accPropX = x; accPropY = y;
	}
	void initDeaccProp(float x, float y) {
		deaccPropX = x; deaccPropY = y;
	}

	void resetProp() {
		propExist = false;
		accPropX = 145; accPropY = -20;
		deaccPropX = 145; deaccPropY = -20;
		showAccProp = false; showDeaccProp = false;
	}

	bool getShowAccProp() { return showAccProp; }
	bool getShowDeaccProp() { return showDeaccProp; }

	void stopPropEffect() { accEffective = false; deaccEffective = false; }

	bool accIsEffective() { return accEffective; }
	bool deaccIsEffective() { return deaccEffective; };

	bool hitAccProp();
	bool hitDeaccProp();
};