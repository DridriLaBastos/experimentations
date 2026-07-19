#ifndef NN_MATH_MAT_HPP
#define NN_MATH_MAT_HPP

#include <memory>
#include <ostream>

#include <stddef.h>

namespace NN::MATH {
template <typename _Type = float>
struct Mat {
    size_t rows;
    size_t cols;
    std::unique_ptr<_Type> data;

    Mat (const size_t r, const size_t c);
    Mat (const size_t r, const size_t c, const _Type x);

    std::ostream& Print(std::ostream& s) const;

    // Utilities functions
    _Type& At (const size_t r, const size_t c) {
        return data.get()[r*cols + c];
    }

    const _Type& At (const size_t r, const size_t c) const {
        return data.get()[r*cols + c];
    }

    static Mat<_Type> Identity (const size_t n);
    static Mat<_Type> AtRandom (const size_t r, const size_t c, const _Type min = 0, const _Type max = 1);

};

template <typename _Type>
Mat<_Type>& Add (const Mat<_Type>& A, const Mat<_Type>& B, Mat<_Type>& dest);

template <typename _Type>
Mat<_Type>& Dot (const Mat<_Type>& A, const Mat<_Type>& B, Mat<_Type>& dest);

}

template <typename _Type>
std::ostream& operator<< (std::ostream& s, const NN::MATH::Mat<_Type>& m);

#include "math/mat.inl"

#endif