// Fall 2022, 24-780-B, Engineering Computation
// Problem Set 2-1: Dice Rolling Monte Carlo
// Create by Zixin Zhang on Sept. 13th, 2022
// Andrew ID: zixinz

#include <iostream>
#include <iomanip>
#include <random>


// generate a random double value given lower bound and upper bound
double gen_rand_value(double lb, double ub) {
	std::random_device rd;  // be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); // standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> unif_dist(lb, ub); // uniform distribution in (lb, ub)
	return unif_dist(gen);
}

// calculate the solution given parameters
double* cal_solution(double massA, double massB, double thetaA1, double thetaB1, double velA1, double velB1, double eRes) {
	static double answer_[4];

	double velAx1 = velA1 * cos(thetaA1 / 180 * 3.142);
	double velAy1 = velA1 * sin(thetaA1 / 180 * 3.142);

	double velBx1 = -velB1 * cos(thetaB1 / 180 * 3.142);
	double velBy1 = -velB1 * sin(thetaB1 / 180 * 3.142);

	double velBx2 = (eRes * (velAx1-velBx1) + massA*velAx1 + massB*velBx1) / (massA+massB);
	double velAx2 = velBx2 - eRes * (velAx1 - velBx1);
	double velAy2 = massA * velAy1 / massA;
	double velBy2 = massB * velBy1 / massB;

	answer_[0] = velAx2;
	answer_[1] = velAy2;
	answer_[2] = velBx2;
	answer_[3] = velBy2;
	
	return answer_;
}

// judge the user's answer
bool grader(double user_answer, double solution, double tol) {
	if (abs(user_answer - solution) <= tol) {
		std::cout << "You are correct! Good job!" << std::endl << std::endl;
		return true;
	}
	else {
		std::cout << "You are wrong :( The correct answer is: " << std::fixed << std::setprecision(3) << solution << std::endl << std::endl;
		return false;
	}
}

int main() {
	
	std::cout << "Two smooth disks A and B, having a mass of 1 kg and 2 kg, respectively, collide with the velocities shown below. If the coefficient of restitution for the disks is e = 0.75, determine the x and y components of the final velocity of each disk just after collision." << std::endl << std::endl;
	
	// generate random parameters
	double massA = gen_rand_value(0.5, 4.0);
	double massB = gen_rand_value(0.5, 4.0);

	double thetaA1 = gen_rand_value(-60.0, 60.0);
	double thetaB1 = gen_rand_value(-60.0, 60.0); 
	
	double velA1 = gen_rand_value(0.0, 5.0);
	double velB1 = gen_rand_value(0.0, 5.0);

	double eRes = gen_rand_value(0.6, 0.9);

	//double massA = 1;
	//double massB = 2;

	//double thetaA1 = 30;
	//double thetaB1 = 45;

	//double velA1 = 3;
	//double velB1 = 1;

	//double eRes = 0.75;

	std::cout << "massA = " << std::fixed << std::setprecision(1) << massA << " kg" << std::endl;
	std::cout << "massB = " << massB << " kg" << std::endl;

	std::cout << "thetaA1 = " << std::fixed << std::setprecision(0) << thetaA1 << " degree" << std::endl;
	std::cout << "thetaB1 = " << thetaB1 << " degree" << std::endl;

	std::cout << "velA1 = " << std::fixed << std::setprecision(1) << velA1 << " m/s" << std::endl;
	std::cout << "velB1 = " << velB1 << " m/s" << std::endl;

	std::cout << "coefficient of restitution = " << std::fixed << std::setprecision(2) << eRes << std::endl << std::endl;

	double *answer = cal_solution(massA, massB, thetaA1, thetaB1, velA1, velB1, eRes); // calculate the solution

	double tol = 0.005; // tolerance

	double user_velAx2, user_velAy2, user_velBx2, user_velBy2;
	std::cout << "Please enter your answer of velAx2: " << std::endl;
	std::cin >> user_velAx2;
	grader(user_velAx2, answer[0], tol);

	std::cout << "Please enter your answer of velAy2: " << std::endl;
	std::cin >> user_velAy2;
	grader(user_velAy2, answer[1], tol);

	std::cout << "Please enter your answer of velBx2: " << std::endl;
	std::cin >> user_velBx2;
	grader(user_velBx2, answer[2], tol);

	std::cout << "Please enter your answer of velBy2: " << std::endl;
	std::cin >> user_velBy2;
	grader(user_velBy2, answer[3], tol);

	return 0;
}
