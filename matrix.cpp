#include "nn.hpp"

#include <random>
#include <cstdio>

template <size_t Row, size_t Column>
Matrix<Row,Column>::Matrix(const Vector& init): mWeights(Row*Column,0)
{
	static_assert((Row != 0) && (Column != 0), "Matrices rows or column number can't be equal to 0");
	size_t weightIndex = 0;

	for (const float& w: init)
	{
		mWeights[weightIndex] = w;
		weightIndex += 1;

		if (weightIndex >= Row*Column)
		{
			break;
		}
	}
}

template <size_t Row, size_t Column>
void Matrix<Row,Column>::operator+=(const Matrix<Row,Column>& m)
{
    const size_t size = Row*Column;

    for (size_t i = 0; i < size; i += 1)
    {
        mWeights[i] += m.mWeights[i];
    }
}

template <size_t Row, size_t Column>
float& Matrix<Row,Column>::operator[] (const size_t i) { return mWeights[i]; }

template <size_t Row, size_t Column>
const float& Matrix<Row,Column>::operator[] (const size_t i) const { return mWeights[i]; }

template <size_t Column>
static constexpr size_t LinearIndex(const size_t x, const size_t y)
{
	return y*Column+x;
};

template <size_t L, size_t M>
Matrix<L,M> operator+(const Matrix<L,M>& A, const Matrix<L,M>& B)
{
	Matrix<L,M> ret;

	for (size_t i = 0; i < L*M; i += 1)
	{
		ret[i] = A[i] + B[i];
	}

	return ret;
}

//TODO: finish implementation
template <size_t L, size_t M, size_t N>
Matrix<L,N> operator* (const Matrix<L,M>& A, const Matrix<M,N>& B)
{
	auto ret = Matrix<L,N>::WithValue(0);

	for (size_t row = 0; row < L; row += 1)
	{
		for (size_t col = 0; col < N; col += 1)
		{
			const size_t retIndex = LinearIndex<N>(row,col);
			float sum = 0;
			for (size_t k = 0; k < M; k += 1)
			{
				const size_t aIndex = LinearIndex<M>(k,row);
				const size_t bIndex = LinearIndex<N>(col,k);
				sum += A[aIndex] * B[bIndex];
			}
			ret[row*N+col] = sum;
		}
	}	

	return ret;
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

template <size_t L, size_t M>
Matrix<L,M> Matrix<L,M>::WithValue(const float x)
{
	Matrix<L,M> m;
	for (float& w: m.mWeights)
	{
		w = x;
	}

	return m;
}
