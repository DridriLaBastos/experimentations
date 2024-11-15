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

static float DeriveW0(const float h, const float w0, const float w1)
{
	return (Cost(w0+h,w1) - Cost(w0,w1))/h;
}

static float DeriveW1(const float h, const float w0, const float w1)
{
	return (Cost(w0,w1+h) - Cost(w0,w1))/h;
}


int main(void)
{
	// Seed with a real random value if available
	std::random_device r;
	std::mt19937 e (r());
	std::uniform_real_distribution<float> d (10,100);

	float w0 = d(e), w1 = d(e);
	const float eps = 1e-3;
	printf("Final error : %.3f\n", Cost(w0-eps*0,w1));

	//Apllying a rate because the value of the derivative might be too big
	const float rate = 1e-3;

	for (size_t i = 0; i < 10000; i += 1)
	{
		const float dW0 = DeriveW0(eps,w0,w1) * rate;
		const float dW1 = DeriveW1(eps,w0,w1) * rate;
		w0 -= dW0;
		w1 -= dW1;
		printf("Final error : %.3f\n", Cost(w0,w1));
	}

	Cost(w0,w1,true);

	return EXIT_SUCCESS;
}
