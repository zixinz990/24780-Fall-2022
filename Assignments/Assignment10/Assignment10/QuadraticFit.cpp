#include "QuadraticFit.h"
#include "fssimplewindow.h"
#include "DrawingUtilNG.h"
#include <iostream>
#include <algorithm>

void QuadraticFit::readFile(std::ifstream& input) {
	Shape2D::readFile(input);
	Shape2D::recalcShape();
}

void QuadraticFit::fit(void) {
	float sumX = 0.0, sumX2 = 0.0, sumX3 = 0.0, sumXY = 0.0, sumY = 0.0, fitY = 0.0;

	for (int i = 0; i < thePoints.size(); i++) {
		sumX = sumX + thePoints[i].x;
		sumX2 = sumX2 + thePoints[i].x * thePoints[i].x;
		sumX3 = sumX3 + thePoints[i].x * thePoints[i].x * thePoints[i].x;
		sumY = sumY + thePoints[i].y;
		sumXY = sumXY + thePoints[i].x * thePoints[i].y;
	}

	coef.a2 = (sumY * sumX2 - sumXY * sumX) / (sumX2 * sumX2 - sumX3 * sumX);
	coef.a1 = (sumY - sumX2 * coef.a2) / sumX;

	curve.resize(thePoints.size());
	for (int i = 0; i < thePoints.size(); i++) {
		float x = (lowerBound.x + i * (upperBound.x - lowerBound.x) / thePoints.size());
		fitY = coef.a1 * x + coef.a2 * x * x;
		curve.at(i) = { x, fitY };
	}
}

void QuadraticFit::paint(bool closed, bool filled, bool showPoints, float colorOverride) {
	if (curve.size() >= 2) {
		double R, G, B;
		if (colorOverride > -1.f) {  // use override color
			DrawingUtilNG::hsv2rgb(colorOverride, 1.f, 1.f, R, G, B);
		}
		else {  // use shape's color
			DrawingUtilNG::hsv2rgb(colorHue, 1.f, 1.f, R, G, B);
		}

		glColor3d(R, G, B);

		if (filled) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4d(R, G, B, 0.5);

			// see Piazza post on using stencils for filling odd shapes
			// https://stackoverflow.com/questions/25422846/how-to-force-opengl-to-draw-a-non-convex-filled-polygon

			glClear(/*GL_COLOR_BUFFER_BIT | */ GL_STENCIL_BUFFER_BIT); // note: only clearing stencil buffer (not colors)
			glEnable(GL_STENCIL_TEST);

			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glStencilFunc(GL_ALWAYS, 0, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
			glStencilMask(1);

			// triangle fan starting at arbritary point (0,0) and using every point in shape, 
			// including closing it by using the first point
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(0., 0.);
			for (auto& currPnt : curve)
				glVertex2f(currPnt.x, currPnt.y);

			// do first point again to "close the loop"
			glVertex2f(curve.front().x, curve.front().y);
			glEnd();

			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glStencilFunc(GL_EQUAL, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

			// draw something over whole area of shape
			glBegin(GL_QUADS);
			glVertex2f(lowerBound.x, lowerBound.y);
			glVertex2f(upperBound.x, lowerBound.y);
			glVertex2f(upperBound.x, upperBound.y);
			glVertex2f(lowerBound.x, upperBound.y);
			glEnd();

			glDisable(GL_STENCIL_TEST);

			// turn off blending 
			glDisable(GL_BLEND);
			glColor3d(R, G, B);

		}

		// whether filled or not, draw the outline 
		// (if filled, draw outline and points in black)
		if (filled) {
			glColor3d(0, 0, 0);  // change color to black
		}

		//Point2D prevPoint = thePoints.front();
		glBegin(GL_LINES);
		glVertex2f(curve.front().x, curve.front().y);

		for (auto currPnt : curve) {
			glVertex2f(currPnt.x, currPnt.y);
			glVertex2f(currPnt.x, currPnt.y);
		}

		if (closed) {
			glVertex2f(curve.front().x, curve.front().y);
		}

		glEnd();

		// now draw the points (maybe)
		if (thePoints.size() >= 2) {
			float pntSize;
			glColor3d(255, 0, 0);
			if (thePoints.size() <= 1) {
				pntSize = 4.f;
			}
			else {
				// set pntSize to 10% of the average line segment length
				pntSize = 0.1 * perimeter() / thePoints.size();
			}
			for (int i = 1; i <= thePoints.size(); i++) {
				paintPoint(i, pntSize);
			}
		}
	}
}

void QuadraticFit::paintPoint(int index, float pntSize) {
	if (1 <= index && index <= thePoints.size()) {
		Point2D currPnt = thePoints.at(index - 1);
		glTranslatef(currPnt.x, currPnt.y, 0.f);

		float adj = getWidth() / getHeight();
		glScalef(adj, 1, 1);
		glBegin(GL_QUADS);
		glVertex2f(pntSize, 0);
		glVertex2f(0, pntSize);
		glVertex2f(-pntSize, 0);
		glVertex2f(0, -pntSize);
		glEnd();
		glScalef(1 / adj, 1, 1);
		glTranslatef(-currPnt.x, -currPnt.y, 0.f);
	}
}