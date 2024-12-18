//
// Created by Adrien COURNAND on 03/08/2024.
//

#include <random>
#include <cstdio>
#include <cmath>

#include "nn.hpp"

static float trainingSet [][3] = {
	{0,0,0},
	{0,1,1},
	{1,0,1},
	{1,1,1},
};

static float Sigmoid (const float x)
{
	return 1.f / (1.f + std::exp(-x));
}

static float Evaluate (const float x0, const float x1, const float w0, const float w1, const float b)
{
	return Sigmoid(w0*x0+w1*x1+b);
}

static float Cost (const float w0, const float w1, const float b, bool verbose = false)
{
	float finalError = 0;

	for (size_t i = 0; i < std::size(trainingSet);i += 1)
	{
		const float expected = trainingSet[i][2];

		const float x0 = trainingSet[i][0];
		const float x1 = trainingSet[i][1];
		const float guess = Evaluate(x0,x1,w0,w1,b);
		const float error = expected - guess;
		finalError += error*error;

		if(verbose)
		{
			printf("(%.3f,%.3f) => %.3f (%.3f) e: %.3f (%.3f)\n",x0,x1,guess, expected,error,finalError);
		}
	}

	return finalError / std::size(trainingSet);
}

static float DeriveW0(const float h, const float w0, const float w1, const float b)
{
	return (Cost(w0+h,w1,b) - Cost(w0,w1,b))/h;
}

static float DeriveW1(const float h, const float w0, const float w1, const float b)
{
	return (Cost(w0,w1+h,b) - Cost(w0,w1,b))/h;
}

static float DeriveB(const float h, const float w0, const float w1, const float b)
{
	return (Cost(w0,w1,b+h) - Cost(w0,w1,b))/h;
}

int main(void)
{
	// Seed with a real random value if available
	std::random_device r;
	std::mt19937 e (r());
	std::uniform_real_distribution<float> d (0,2);

	float w0 = d(e), w1 = d(e), b = d(e);
	const float eps = 1e-2;

	//Apllying a rate because the value of the derivative might be too big
	const float rate = 1e-1;

	for (size_t i = 0; i < 10000; i += 1)
	{
		const float dW0 = DeriveW0(eps,w0,w1,b) * rate;
		const float dW1 = DeriveW1(eps,w0,w1,b) * rate;
		const float db = DeriveB(eps,w0,w1,b) * rate;
		w0 -= dW0;
		w1 -= dW1;
		b -= db;
		printf("Final error : %.3f\n", Cost(w0,w1,b));
	}

	Cost(w0,w1,b,true);

	Layer l;

	return EXIT_SUCCESS;
}
