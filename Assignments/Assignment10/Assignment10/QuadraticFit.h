#pragma once

#include <fstream>
#include "Shape2D.h"

struct QuadraticCoef {
	float a0;
	float a1;
	float a2;
};

class QuadraticFit : public Shape2D {
public:
	std::vector<Point2D> curve;
	QuadraticCoef coef;

	void readFile(std::ifstream& input);

	void fit(void);

	void paint(bool closed = true, bool filled = false, bool showPoints = false, float colorOverride = -1.f);

	void paintPoint(int index, float pntSize = 0.f);

	virtual bool selfIntersects() { return false; };
};