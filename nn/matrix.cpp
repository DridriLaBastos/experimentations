#ifndef NN_HEADER_ONLY
#include "nn.hpp"
#endif

template <unsigned int Row, unsigned int Column>
Matrix<Row,Column>::Matrix(): mWeights(MallocHelper<float>(Row*Column),MallocDeleter<float>())
{
}

template <unsigned int Row, unsigned int Column>
void Matrix<Row,Column>::operator+=(const Matrix<Row,Column>& m)
{
    const size_t size = Row*Column;

    for (size_t i = 0; i < size; i += 1)
    {
        mWeights.get()[i] += m.mWeights.get()[i];
    }
}
