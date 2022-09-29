#include "Shape2D.h"

Shape2D::Shape2D(std::ifstream& input)
{
	// these will temporarily hold the values read from the file
	float newX, newY;

	// these are used to avoid duplicate points, initialized to values that
	// essentially mean "no point at all" (the negative of largest possible float number)
	float oldX = -INFINITY, oldY = -INFINITY;

	while (!input.eof()) { // continue reading until the whole file has been read

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

bool Shape2D::addPoint(Point2D newPoint, int index) {
	try {
		if (index <= thePoints.size()) {
			thePoints.insert(thePoints.begin() + index - 1, newPoint);
			return true;
		}
		else { // if the index is greater than the number of points
			thePoints.push_back(newPoint);
			return true;
		}
	}
	catch (...) {
		std::cout << "Something wrong when try adding the point." << std::endl;
		return false;
	}
}

bool Shape2D::addPoint(float newX, float newY, int index) {
	try {
		Point2D newPoint;
		newPoint.x = newX;
		newPoint.y = newY;

		if (index <= thePoints.size()) {
			thePoints.insert(thePoints.begin() + index - 1, newPoint);
			return true;
		}
		else { // if the index is greater than the number of points
			thePoints.push_back(newPoint);
			return true;
		}
	}
	catch (...) {
		std::cout << "Something wrong when try adding the point." << std::endl;
		return false;
	}
}

bool Shape2D::addPoint(int index, float ratio) {
	try {
		if (ratio >= 0 || ratio <= 1) {
			Point2D startPoint = thePoints[index - 1];
			Point2D endPoint = thePoints[index];
			Point2D newPoint;

			newPoint.x = startPoint.x + ratio * (endPoint.x - startPoint.x);
			newPoint.y = startPoint.y + ratio * (endPoint.y - startPoint.y);

			if (index <= thePoints.size()) {
				thePoints.insert(thePoints.begin() + index - 1, newPoint);
				return true;
			}
			else { // if the index is greater than the number of points
				thePoints.push_back(newPoint);
				return true;
			}
		}
		else {
			throw("error");
		}
	}
	catch (...) {
		std::cout << "Something wrong when try adding the point." << std::endl;
		return false;
	}
}

bool Shape2D::removePoint(int index) {
	try {
		if (index <= thePoints.size()) {
			thePoints.erase(thePoints.begin() + index - 1);
			return true;
		}
		else { // if the index is greater than the number of points
			throw("error");
		}
	}
	catch (...) {
		std::cout << "Something wrong when try removing the point." << std::endl;
		return false;
	}
}

void Shape2D::paint(bool closed, bool filled) {
	for (int i = 0; i < thePoints.size(); i++) {
		glBegin(GL_LINES);

		if (i <= thePoints.size() - 2) {
			glVertex2f(thePoints[i].x, thePoints[i].y);
			glVertex2f(thePoints[i + 1].x, thePoints[i + 1].y);
		}
		else { // connect back to the first point
			glVertex2f(thePoints[i].x, thePoints[i].y);
			glVertex2f(thePoints[0].x, thePoints[0].y);
		}

		glEnd();
		glFlush();
		FsPollDevice();
		FsSleep(10);
	}
}

float Shape2D::perimeter() {
	float perimeter = 0.0;
	for (int i = 0; i < thePoints.size(); i++) {
		if (i <= thePoints.size() - 2) {
			perimeter = perimeter + Line2D::getLength(thePoints[i], thePoints[i + 1]);
		}
		else { // connect back to the first point
			perimeter = perimeter + Line2D::getLength(thePoints[0], thePoints[i]);
		}
	}
	return perimeter;
}

std::ostream& operator<<(std::ostream& os, const Shape2D& aShape) {
	for (int i = 0; i < aShape.thePoints.size(); i++) {
		os << aShape.thePoints[i].x << "\t" << aShape.thePoints[i].y << std::endl;
	}
	return os;
}