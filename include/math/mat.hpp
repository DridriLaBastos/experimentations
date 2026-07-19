#ifndef NN_MATH_MAT_HPP
#define NN_MATH_MAT_HPP

#include <initializer_list>
#include <memory>
#include <ostream>

#include <stddef.h>

namespace MATH {
template <typename _Type = float>
struct Mat {
private:
    std::unique_ptr<_Type[]> managedData;

public:
    size_t rows;
    size_t cols;
    _Type* data;

    Mat (const size_t r, const size_t c, _Type* initData = nullptr);
    Mat (const size_t r, const size_t c, const _Type x);
    Mat (const size_t r, const size_t c, std::initializer_list<_Type> initData);

    std::ostream& Print(std::ostream& s) const;

    // Utilities functions
    _Type& At (const size_t r, const size_t c) {
        return data[r*cols + c];
    }

    const _Type& At (const size_t r, const size_t c) const {
        return data[r*cols + c];
    }

    static Mat<_Type> Identity (const size_t n);
    static Mat<_Type> AtRandom (const size_t r, const size_t c, const _Type min = 0, const _Type max = 1);
};

template <typename _Type>
Mat<_Type>& Add (const Mat<_Type>& A, const Mat<_Type>& B, Mat<_Type>& dest);

template <typename _Type>
Mat<_Type>& Dot (const Mat<_Type>& A, const Mat<_Type>& B, Mat<_Type>& dest);

template <typename _Type>
Mat<_Type>& ApplyFunction (_Type(*func)(_Type), MATH::Mat<_Type>& M);

}

template <typename _Type>
std::ostream& operator<< (std::ostream& s, const MATH::Mat<_Type>& m);

#include "math/mat.inl"

#endif