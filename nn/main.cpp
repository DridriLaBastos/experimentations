//
// Created by Adrien COURNAND on 03/08/2024.
//

#include <random>
#include <cstdio>

static float trainingSet [][2] = {
	{0,1},
	{1,3},
	{2,5},
	{3,7},
	{4,9},
};

static float Cost (const float w0, const float w1, const bool verbose = false)
{
	float finalError = 0;

	for (size_t i = 0; i < std::size(trainingSet);i += 1)
	{
		const float expected = trainingSet[i][1];

		const float x = trainingSet[i][0];
		const float guess = w1*x + w0;
		const float error = expected - guess;
		finalError += error*error;

		if(verbose)
		{
			printf("%.3f => %.3f (%.3f) e: %.3f (%.3f)\n",x,guess, expected,error,finalError);
		}
	}

	return finalError / std::size(trainingSet);
}

int main(void)
{
	// Seed with a real random value if available
	std::random_device r;
	std::mt19937 e (r());
	std::uniform_real_distribution<float> d (10,100);

	float w0 = d(e), w1 = d(e);
	printf("Final error : %.3f\n", Cost(w0,w1));

	return EXIT_SUCCESS;
}
