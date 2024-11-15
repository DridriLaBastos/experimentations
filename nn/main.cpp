//
// Created by Adrien COURNAND on 03/08/2024.
//

#include <random>
#include <iostream>

int main(void)
{
	// Seed with a real random value if available
	std::random_device r;
	std::mt19937 e (r());
	std::uniform_real_distribution<float> d;
	std::cout << d(e) << std::endl;
	return EXIT_SUCCESS;
}

