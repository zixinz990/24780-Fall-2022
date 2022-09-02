// Fall 2022, 24-780-B, Engineering Computation
// Problem Set 1: PI the Hard Way
// Create by Zixin Zhang on 09/01/2022
// Andrew ID: zixinz

#include <math.h>
#include <iostream>

double approximatePi(double dx) {
	double area = 0.0;
	int n = int(1.0 / dx);
	double d = 0.0;

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			d = dx * (i - 1.0) * dx * (i - 1.0) + dx * (j - 1.0) * dx * (j - 1.0);
			if (d < 1.0) {
				area += dx * dx;
			}
		}
	}

	return 4 * area;
}

int main() {
	double real_pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;

	double dx = 0.001;
	double approximated_pi = approximatePi(dx);
	double error = approximated_pi - real_pi;
	std::cout << "PI is approximated to be " << approximated_pi << " when dx = " << dx << "." << std::endl;
	std::cout << "The error is about " << error << " (" << 100 * error / real_pi << "%)." << std::endl << std::endl;

	dx = 0.0001;
	approximated_pi = approximatePi(dx);
	error = approximated_pi - real_pi;
	std::cout << "PI is approximated to be " << approximated_pi << " when dx = " << dx << "." << std::endl;
	std::cout << "The error is about " << error << " (" << 100 * error / real_pi << "%)." << std::endl << std::endl;

	dx = 0.00001;
	approximated_pi = approximatePi(dx);
	error = approximated_pi - real_pi;
	std::cout << "PI is approximated to be " << approximated_pi << " when dx = " << dx << "." << std::endl;
	std::cout << "The error is about " << error << " (" << 100 * error / real_pi << "%)." << std::endl << std::endl;

	return 0;
}
