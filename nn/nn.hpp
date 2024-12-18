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

template <size_t Row, size_t Column>
class Matrix
{
    public:
        Matrix(void);

        void operator+= (const Matrix& m);
		void Print(void) const;
    
    private:
        MallocDeleterUniquePtr<float> mWeightsPtr;
		float* mWeights;
};

template <size_t L, size_t M, size_t N>
Matrix <L,N> operator* (const Matrix<L,M>& A, const Matrix<M,N>& B);

#if NN_HEADER_ONLY
#include "matrix.cpp"
#endif

#endif