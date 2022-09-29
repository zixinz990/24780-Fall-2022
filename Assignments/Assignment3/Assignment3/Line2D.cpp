#include "Line2D.h"

double Line2D::getLength(Point2D startPoint, Point2D endPoint) {
	double length = 0.0;
	length = sqrt((startPoint.x - endPoint.x) * (startPoint.x - endPoint.x) + (startPoint.y - endPoint.y) * (startPoint.y - endPoint.y));
	return length;
}

double Line2D::getLength(float startX, float startY, float endX, float endY) {
	double length = 0.0;
	length = sqrt((startX - endX) * (startX - endX) + (startY - endY) * (startY - endY));
	return length;
}

bool Line2D::isBetween(Point2D startPoint, Point2D endPoint, Point2D checkPoint) {
	double length = getLength(startPoint, endPoint);
	if (getLength(checkPoint, startPoint) > length || getLength(checkPoint, endPoint) > length) {
		return false;
	}
	else {
		return true;
	}
}

Point2D Line2D::getIntersection(Point2D lineAstart, Point2D lineAend, Point2D lineBstart, Point2D lineBend) {
	double kA = (lineAstart.y - lineAend.y) / (lineAstart.x - lineAend.x);
	double kB = (lineBstart.y - lineBend.y) / (lineBstart.x - lineBend.x);
	Point2D intersection;
	intersection.x = (lineBstart.y - lineAstart.y + kA * lineAstart.x - kB * lineBstart.x) / (kA - kB);
	intersection.y = lineAstart.y + kA * (intersection.x - lineAstart.x);
	return intersection;
}