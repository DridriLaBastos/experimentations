//
// Created by Adrien COURNAND on 03/08/2024.
//

#include <iostream>

float trainSet [4][2] = {
		{1},{2},{3},{4},
};

template <size_t _M, size_t _N, typename _T>
static void Fill (const double a, const double b, _T in_out [_M][_N])
{
	for (size_t i = 0; i < _M; i += 1)
	{
		in_out[i][1] = a * in_out[i][0] + b;
	}
}

int main(void)
{
	Fill<4,2>(2,0,trainSet);

	for (auto trainEntry: trainSet)
	{
		printf("TrainSer %.2f -> %.2f\n",trainEntry[0],trainEntry[1]);
	}

	std::cout << "Hello neural network world\n";
	return EXIT_SUCCESS;
}
