#include <iostream>
#include <math.h>
#include "fssimplewindow.h"

#include "Shape2D.h"

using namespace std;

Shape2D::Shape2D()
{
	// nothing here yet
}

Shape2D::Shape2D(std::ifstream& input)
{
	// these will temporarily hold the values read from the file
	float newX, newY;

	// these are used to avoid duplicate points, initialized to values that
	// essentially mean "no point at all" (the negative of largest possible float number)
	float oldX = -INFINITY, oldY = -INFINITY;

	while (!input.eof()) {  // continue reading until the whole file has been read

		// the ifstream variable "input" can be used just like "cin".
		// it is like magic in that it does all the "translating" and advancing
		// down the line and down the file, using tabs, spaces, and newline
		// characters as separators.
		input >> newX >> newY;

		// we want to avoid the consecutive points being exactly the same
		if (newX != oldX || newY != oldY)
			thePoints.push_back({ newX, newY }); // define Point2D on the fly & add to vector

		// reset old coords in preparation for reading next coords
		oldX = newX; oldY = newY;
	}
}

bool Shape2D::addPoint(Point2D newPoint, int index)
{
	if (index < 1)
		return false;
	else {
		if (index > thePoints.size())
			thePoints.push_back(newPoint);
		else
			thePoints.insert(thePoints.begin() + index - 1, newPoint);

		return true;
	}
}

bool Shape2D::addPoint(int index, float ratio)
{
	if (0. <= ratio || ratio >= 1.0 || index < 2 || index >= thePoints.size())
		return false;
	else {
		// insert a point between index-2 and index-1
		// the coords of new point are based on similar triangles
		// coords = prevPoint + ratio * (difference from nextPoint to newPoint)
		float newX = thePoints[index - 2].x
			+ ratio * (thePoints[index - 1].x - thePoints[index - 2].x);
		float newY = thePoints[index - 2].y
			+ ratio * (thePoints[index - 1].y - thePoints[index - 2].y);
		thePoints.insert(thePoints.begin() + index - 1, { newX, newY });
	}
}

bool Shape2D::removePoint(int index)
{
	if (index < 1 || index > thePoints.size())
		return false;
	else {
		thePoints.erase(thePoints.begin() + index - 1);
		return true;
	}
}

void Shape2D::paint(bool closed, bool filled, bool showPoints, float colorOverride)
{
	if (thePoints.size() >= 2) {
		//Point2D prevPoint = thePoints.front();
		glBegin(GL_LINES);
		glVertex2f(thePoints.front().x, thePoints.front().y);

		for (auto currPnt : thePoints) {
			glVertex2f(currPnt.x, currPnt.y);
			glVertex2f(currPnt.x, currPnt.y);
		}
		if (closed)
			glVertex2f(thePoints.front().x, thePoints.front().y);
		glEnd();

		if (showPoints) {
			float pntSize;
			if (thePoints.size() <= 1)
				pntSize = 4.f;
			else
				// set pntSize to 10% of the average line segment length
				pntSize = 0.1 * perimeter() / thePoints.size();

			for (auto currPnt : thePoints) {
				glTranslatef(currPnt.x, currPnt.y, 0.f);

				glBegin(GL_QUADS);
				glVertex2f(pntSize, 0);
				glVertex2f(0, pntSize);
				glVertex2f(-pntSize, 0);
				glVertex2f(0, -pntSize);
				glEnd();

				glTranslatef(-currPnt.x, -currPnt.y, 0.f);
			}
		}
	}
}

//float Shape2D::perimeter()
//{
//	if (thePoints.size() >= 3) { // need at least three points to make sense
//
//		float result = 0.f;
//		Point2D prevPnt = thePoints.back();
//		for (auto currPnt : thePoints) {
//			result += Line2D::getLength(prevPnt, currPnt);
//			prevPnt = currPnt;
//		}
//		return result;
//	}
//	else
//		return 0.f;  // essentially error
//
//}

bool Shape2D::isInSquare(Point2D testPoint, Point2D targetPoint, float dist)
{
	// using what I learned from spaceships and targets
	return abs(testPoint.x - targetPoint.x) <= dist && abs(testPoint.y - targetPoint.y) <= dist;

	// this works too, but is more complicated, essentially "check if testpoint is outside square"
	//return !(testPoint.x < (targetPoint.x - dist) || testPoint.x >(targetPoint.x + dist)
	// || testPoint.y < (targetPoint.y - dist) || testPoint.y >(targetPoint.y + dist));
}

std::ostream& operator<<(std::ostream& os, const Shape2D& aShape)
{
	for (auto currPnt : aShape.thePoints) {
		os << currPnt.x << "\t" << currPnt.y << endl;
	}
	return os;
}
