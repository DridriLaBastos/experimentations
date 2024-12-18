#ifndef NN_HPP
#define NN_HPP

#include <memory>
#include <cstdlib>

template <typename _T>
struct MallocDeleter {
    void operator()(_T* ptr)
    {
        free(ptr);
        ptr = nullptr;
    }
};

template<typename _T>
_T* MallocHelper(const size_t arrayCount = 1)
{
    return (_T*)malloc(sizeof(_T)*arrayCount);
}

template <typename _T>
using MallocDeleterUniquePtr = std::unique_ptr<_T,MallocDeleter<_T> >;

template <unsigned int Row, unsigned int Column>
class Matrix
{
    public:
        Matrix(void);

        void operator+= (const Matrix& m);
    
    private:
        MallocDeleterUniquePtr<float> mWeights;
};

#if NN_HEADER_ONLY
#include "matrice.cpp"
#endif

#endif