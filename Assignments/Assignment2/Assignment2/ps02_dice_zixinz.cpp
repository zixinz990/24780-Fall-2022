// Fall 2022, 24-780-B, Engineering Computation
// Problem Set 2-1: Dice Rolling Monte Carlo
// Create by Zixin Zhang on Sept. 12th, 2022
// Andrew ID: zixinz

#include <iostream>

void roll(const int roll_times) {
	int results[11]; // store results, each element is the count of numbers

	// initialize the results array
	for (int i = 0; i < 11; i++) {
		results[i] = 0;
	}

	// initialize some variables
	int range = 6 - 1 + 1;
	int num_1 = 0;
	int num_2 = 0;
	int result = 0;

	for (int i = 0; i < roll_times; i++) {
		num_1 = std::rand() % range + 1; // dice 1
		num_2 = std::rand() % range + 1; // dice 2
		result = num_1 + num_2; // get result, a number in [2, 12]

		results[result - 2] += 1; // count
	}

	// print results
	std::cout << "Results (each '*' represents 1%): " << std::endl;	
	for (int i = 0; i < 11; i++) {
		std::cout.width(3);
		std::cout << i+2 << ": " << std::string(results[i], '*') << std::endl;
	}
}

int main() {
	// std::cout << std::string(5, '*') << std::endl;
	std::cout << "============ DICE ROLLING ==============" << std::endl;
	std::cout << "Enter number of rolls > 50" << std::endl;
	
	int roll_times;
	std::cin >> roll_times;
	roll(roll_times);

	return 0;
}
