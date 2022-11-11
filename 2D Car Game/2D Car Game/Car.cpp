#include "Car.h"

using namespace std;

void Car::reset() {
	curPos = startPos;
	curVel = { 0, 0 };
	curAcc = { 0, 0 };
	curState = DEFAULT_STATE;
}

bool Car::genMoveTraj(Vector tgtPos, float T) {
	float x0 = curPos.x;
	//float y0 = curPos.y;
	float vx0 = curVel.x;
	//float vy0 = curVel.y;

	float distanceX = tgtPos.x - x0;
	//float distanceY = tgtPos.y - y0;

	float maxVelX = (distanceX - vx0 * T) / (T / 2 - DT);
	//float maxVelY = (distanceY - vy0 * T) / (T / 2 - DT);

	int n = T / DT;

	float xt = x0;
	//float yt = y0;
	float vxt = vx0;
	//float vyt = vy0;

	float curT = 0;

	for (int i = 0; i < n; i++) {
		if (i < n / 2) {
			vxt = vx0 + 2 * maxVelX * curT / T;
			//vyt = vy0 + 2 * maxVelY * curT / T;
		}
		else {
			vxt = maxVelX - 2 * maxVelX / T * (curT + DT - T / 2);
			//vyt = maxVelY - 2 * maxVelY / T * (curT + DT - T / 2);
		}
		xt = xt + vxt * DT;
		//yt = yt + vyt + DT;

		//float arr[5] = { xt, yt, vxt, vyt, curT };
		//traj.push_back(arr);
		xTraj.push_back(xt);

		curT = curT + DT;
	}
	xTraj.push_back(tgtPos.x);

	return true;
}

bool Car::accel(Vector acc) {
	curVel.x = curVel.x + acc.x * DT;
	curVel.y = curVel.y + acc.y * DT;

	curPos.x = curPos.x + curVel.x * DT; // only move the car in x direction

	return true;
}

void Car::paint() {
	using namespace DrawingUtilNG;

	// paint car body
	glColor3ub(color.R, color.G, color.B);
	drawRectangle(curPos.x - width / 2, curPos.y - length / 2, width, length, true);

	// paint tires
	glColor3ub(0, 0, 0);
	drawRectangle(curPos.x - width / 2 - 5, curPos.y - length / 2 + 10, 5, 15, true);
	drawRectangle(curPos.x - width / 2 - 5, curPos.y + length / 2 - 25, 5, 15, true);
	drawRectangle(curPos.x + width / 2, curPos.y - length / 2 + 10, 5, 15, true);
	drawRectangle(curPos.x + width / 2, curPos.y + length / 2 - 25, 5, 15, true);

	// paint window
	drawRectangle(curPos.x - 15, curPos.y - 15, 30, 40, true);
	drawRectangle(curPos.x - width / 2, curPos.y - length / 2, 15, 5, true);
	drawRectangle(curPos.x + width / 2 - 15, curPos.y - length / 2, 15, 5, true);
}
