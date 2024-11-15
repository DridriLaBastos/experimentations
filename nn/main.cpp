//
// Created by Adrien COURNAND on 03/08/2024.
//

#include <random>
#include <cstdio>

static float trainingSet [][3] = {
	{0,0,1},
	{0,1,2},
	{1,0,2},
	{1,1,2},
};

static float Cost (const float w0, const float w1, const float w2, bool verbose = false)
{
	float finalError = 0;

	for (size_t i = 0; i < std::size(trainingSet);i += 1)
	{
		const float expected = trainingSet[i][2];

		const float x0 = trainingSet[i][0];
		const float x1 = trainingSet[i][1];
		const float guess = w0*x0 + w1*x1 + w2;
		const float error = expected - guess;
		finalError += error*error;

		if(verbose)
		{
			printf("(%.3f,%.3f) => %.3f (%.3f) e: %.3f (%.3f)\n",x0,x1,guess, expected,error,finalError);
		}
	}

	return finalError / std::size(trainingSet);
}

static float DeriveW0(const float h, const float w0, const float w1, const float w2)
{
	return (Cost(w0+h,w1,w2) - Cost(w0,w1,w2))/h;
}

static float DeriveW1(const float h, const float w0, const float w1, const float w2)
{
	return (Cost(w0,w1+h,w2) - Cost(w0,w1,w2))/h;
}

static float DeriveW2(const float h, const float w0, const float w1, const float w2)
{
	return (Cost(w0,w1,w2+h) - Cost(w0,w1,w2))/h;
}

int main(void)
{
	// Seed with a real random value if available
	std::random_device r;
	std::mt19937 e (r());
	std::uniform_real_distribution<float> d (10,100);

	float w0 = d(e), w1 = d(e), w2 = d(e);
	const float eps = 2e-3;

	//Apllying a rate because the value of the derivative might be too big
	const float rate = 1e-3;

	for (size_t i = 0; i < 10000; i += 1)
	{
		const float dW0 = DeriveW0(eps,w0,w1,w2) * rate;
		const float dW1 = DeriveW1(eps,w0,w1,w2) * rate;
		const float dW2 = DeriveW2(eps,w0,w1,w2) * rate;
		w0 -= dW0;
		w1 -= dW1;
		w2 -= dW2;
		printf("Final error : %.3f\n", Cost(w0,w1,w2));
	}

	Cost(w0,w1,w2,true);

	return EXIT_SUCCESS;
}
