#pragma once

#define DEFAULT_START_X 145
#define DEFAULT_START_Y 600

#define WINDOW_X_SIZE 600
#define WINDOW_Y_SIZE 800

#define DEFAULT_VEL_X 0
#define DEFAULT_VEL_Y 0.5 // pixel/ms, road

#define DEFAULT_ACC_X 0
#define DEFAULT_ACC_Y 0

#define DEFAULT_STATE 0

#define DT 10.0

struct Vector {
	float x, y;
};

struct Color {
	float R, G, B;
};