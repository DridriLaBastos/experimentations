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

    std::ostream& Print(std::ostream& s) const;

    static Mat<_Type> AtRandom (const size_t r, const size_t c, const _Type min = 0, const _Type max = 1);

    static Mat<_Type>& Dot (Mat<_Type>& res, const Mat<_Type>& a, const Mat<_Type>& b);

private:
    // Utilities functions
    _Type& At (const size_t r, const size_t c) {
        return data.get()[r*cols + c];
    }

    const _Type& At (const size_t r, const size_t c) const {
        return data.get()[r*cols + c];
    }

};

}

template <typename _Type>
std::ostream& operator<< (std::ostream& s, const NN::MATH::Mat<_Type>& m);

#include "math/mat.inl"

#endif