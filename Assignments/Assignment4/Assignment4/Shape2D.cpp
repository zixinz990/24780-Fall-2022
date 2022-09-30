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

	recalcShape();
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

		recalcShape();
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

		recalcShape();
		return true;
	}
}

bool Shape2D::removePoint(int index)
{
	if (index < 1 || index > thePoints.size())
		return false;
	else {
		thePoints.erase(thePoints.begin() + index - 1);

		recalcShape();
		return true;
	}
}

void Shape2D::paint(bool closed, bool filled, bool showPoints, float colorOverride)
{
	if (thePoints.size() >= 2) {

		if (colorOverride != -1) { // display the shape's own color
			double R, G, B;
			colorOverride = colorHue; // the initial value of colorHue is 0
			DrawingUtilNG::hsv2rgb(colorHue, 1., 1., R, G, B);
			glColor3d(R, G, B);
		}
		else { // display black
			glColor3d(0, 0, 0);
		}

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

std::ostream& operator<<(std::ostream& os, const Shape2D& aShape)
{
	for (auto currPnt : aShape.thePoints) {
		os << currPnt.x << "\t" << currPnt.y << endl;
	}
	return os;
}

Point2D Shape2D::getLowerBound()
{
	lowerBound.x = thePoints[0].x;
	lowerBound.y = thePoints[0].y;
	for (int i = 1; i < thePoints.size(); i++) {
		if (thePoints[i].x < lowerBound.x) {
			lowerBound.x = thePoints[i].x;
		}
		if (thePoints[i].y < lowerBound.y) {
			lowerBound.y = thePoints[i].y;
		}
	}
	return lowerBound;
}

Point2D Shape2D::getUpperBound()
{
	upperBound.x = thePoints[0].x;
	upperBound.y = thePoints[0].y;
	for (int i = 1; i < thePoints.size(); i++) {
		if (thePoints[i].x > upperBound.x) {
			upperBound.x = thePoints[i].x;
		}
		if (thePoints[i].y > upperBound.y) {
			upperBound.y = thePoints[i].y;
		}
	}
	return upperBound;
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
//	else {
//		return 0.f;  // essentially error
//	}	
//}

Point2D Shape2D::getPoint(int index)
{
	if (index >= 1 && index <= thePoints.size()) { // if the index is legal
		return thePoints[index - 1];
	}
	else {
		return { -INFINITY, -INFINITY };
	}
}

bool Shape2D::movePoint(Point2D newCoords, int index)
{
	if (index >= 1 && index <= thePoints.size()) { // if the index is legal
		thePoints[index - 1] = newCoords;
		return true;
	}
	else {
		return false;
	}
}

bool Shape2D::movePointDelta(Point2D deltaCoords, int index)
{
	if (index >= 1 && index <= thePoints.size()) {  // if the index is legal
		thePoints[index - 1].x = thePoints[index - 1].x + deltaCoords.x;
		thePoints[index - 1].y = thePoints[index - 1].y + deltaCoords.y;
		return true;
	}
	else {
		return false;
	}
}

int Shape2D::getIndex(Point2D givenCoord, float dist)
{
	if (givenCoord.x < lowerBound.x || givenCoord.y < lowerBound.y || givenCoord.x > upperBound.x || givenCoord.y > upperBound.y) { // if the givenCoord is illegal
		return -1;
	}
	else {
		for (int i = 0; i < thePoints.size(); i++) {
			if (isInSquare(thePoints[i], givenCoord, dist)) {
				return i + 1;
			}
		}
	}
	return -1;
}

bool Shape2D::selfIntersects()
{
	Point2D intersection;
	intersection.x = -INFINITY;
	intersection.y = -INFINITY;
	Point2D prevPnt = thePoints.back(); // the first line we want to check is from -1 -> 0
	for (int i = 0; i < thePoints.size() - 2; i++) {
		if (i == 0) {
			for (int j = 2; j < thePoints.size() - 1; j++) { // check i-1 -> i and j-1 -> j; don't check neighbouring lines
				intersection = Line2D::getTrueIntersection(prevPnt, thePoints[i], thePoints[j - 1], thePoints[j]);
			}
		}
		else {
			for (int j = i + 2; j < thePoints.size(); j++) { // check i-1 -> i and j-1 -> j; don't check neighbouring lines
				intersection = Line2D::getTrueIntersection(prevPnt, thePoints[i], thePoints[j - 1], thePoints[j]);
			}
		}
		prevPnt = thePoints[i];
	}
	if (intersection.x == -INFINITY && intersection.y == -INFINITY) {
		return false;
	}
	else {
		return true;
	}
}

void Shape2D::recalcShape()
{
	Point2D prevPnt = thePoints.back();

	if (thePoints.size() >= 3) { // need at least 3 points to calculate perimeter
		for (auto currPnt : thePoints) { // update lb and ub
			perim += Line2D::getLength(prevPnt, currPnt);
			if (currPnt.x < lowerBound.x && currPnt.x > -INFINITY) {
				lowerBound.x = currPnt.x;
			}
			if (currPnt.y < lowerBound.y && currPnt.y > -INFINITY) {
				lowerBound.y = currPnt.y;
			}
			if (currPnt.x > upperBound.x && currPnt.x < INFINITY) {
				upperBound.x = currPnt.x;
			}
			if (currPnt.y > upperBound.y && currPnt.y < INFINITY) {
				upperBound.y = currPnt.y;
			}
			prevPnt = currPnt;
		}
	}
	else { // don't have enough points, can't calculate perim
		perim = 0.f;
		for (auto currPnt : thePoints) { // update lb and ub
			if (currPnt.x < lowerBound.x && currPnt.x > -INFINITY) {
				lowerBound.x = currPnt.x;
			}
			if (currPnt.y < lowerBound.y && currPnt.y > -INFINITY) {
				lowerBound.y = currPnt.y;
			}
			if (currPnt.x > upperBound.x && currPnt.x < INFINITY) {
				upperBound.x = currPnt.x;
			}
			if (currPnt.y > upperBound.y && currPnt.y < INFINITY) {
				upperBound.y = currPnt.y;
			}
			prevPnt = currPnt;
		}
	}
}

bool Shape2D::isInSquare(Point2D testPoint, Point2D targetPoint, float dist)
{
	// using what I learned from spaceships and targets
	return abs(testPoint.x - targetPoint.x) <= dist && abs(testPoint.y - targetPoint.y) <= dist;

	// this works too, but is more complicated, essentially "check if testpoint is outside square"
	//return !(testPoint.x < (targetPoint.x - dist) || testPoint.x >(targetPoint.x + dist)
	// || testPoint.y < (targetPoint.y - dist) || testPoint.y >(targetPoint.y + dist));
}