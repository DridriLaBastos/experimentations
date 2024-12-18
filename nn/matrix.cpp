#ifndef NN_HEADER_ONLY
#include "nn.hpp"
#endif

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
