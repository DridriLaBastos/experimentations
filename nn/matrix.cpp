#ifndef NN_HEADER_ONLY
#include "nn.hpp"
#endif

#include <random>
#include <cstdio>

template <size_t Row, size_t Column>
Matrix<Row,Column>::Matrix(): mWeightsPtr(MallocHelper<float>(Row*Column),MallocDeleter<float>()), mWeights(nullptr)
{
	static_assert((Row != 0) && (Column != 0), "Matrices rows or column number can't be equal to 0");
	mWeights = mWeightsPtr.get();
}

template <size_t Row, size_t Column>
void Matrix<Row,Column>::operator+=(const Matrix<Row,Column>& m)
{
    const size_t size = Row*Column;

    for (size_t i = 0; i < size; i += 1)
    {
        mWeights[i] += m.mWeights.get()[i];
    }
}

template <size_t L, size_t M, size_t N>
Matrix <L,N> operator* (const Matrix<L,M>& A, const Matrix<M,N>& B)
{
	return Matrix<L,M>();
}

template <size_t L, size_t M>
void Matrix<L,M>::Print() const
{
	for(size_t i = 0; i < L; i+=1)
	{
		for (size_t j = 0; j < M; j++)
		{
			const size_t linearIndex = i*M+j;
			printf("%.3f ",mWeights[linearIndex]);
		}
		puts("");
	}
}

/// Static functions
template <size_t L, size_t M>
Matrix<L,M> Matrix<L,M>::WithRandom(const float min, const float max)
{
	// Seed with a real random value if available
	std::random_device r;
	std::mt19937 e (r());
	std::uniform_real_distribution<float> d (min,max);
	Matrix<L,M> m;

	for(size_t i = 0; i < m.SIZE; i += 1)
	{
		m.mWeights[i] = d(e);
	}

	return m;
}
