#include <format>
#include <stdexcept>

#include "math/mat.hpp"
#include "math/util.hpp"

template<typename _Type>
NN::MATH::Mat<_Type>::Mat(const size_t r, const size_t c): rows{r}, cols{c}, data{ new _Type[r*c] }
{
    //TODO: Maybe throw of allocation fails ?
}

template<typename _Type>
NN::MATH::Mat<_Type>::Mat (const size_t r, const size_t c, const _Type x): Mat<_Type>(r,c)
{
    for (size_t i = 0; i < r*c; i+= 1)
    {
        this->data.get()[i] = x;
    }
}

template <typename _Type>
NN::MATH::Mat<_Type> NN::MATH::Mat<_Type>::AtRandom (const size_t r, const size_t c, const _Type min, const _Type max)
{
    NN::MATH::Mat<_Type> m (r,c);

    //TODO: We probably want to have iterators here
    for (size_t i = 0; i < r*c; i += 1) {
        m.data.get()[i] = GetRandom<_Type>(min,max);
    }

    return m;
}

template<typename _Type>
NN::MATH::Mat<_Type>& NN::MATH::Mat<_Type>::Dot (NN::MATH::Mat<_Type>& res, const NN::MATH::Mat<_Type>& a, const NN::MATH::Mat<_Type>& b)
{
    
}

template <typename _Type>
std::ostream& NN::MATH::Mat<_Type>::Print(std::ostream& s) const
{
    for (size_t r = 0; r < this->rows; r += 1)
    {
        for (size_t c = 0; c < this->cols; c += 1) {
            s << std::format("{:4} ", this->At(r,c));
        }
        s << std::endl;
    }

    return s;
}

template <typename _Type>
std::ostream& operator<< (std::ostream& s, const NN::MATH::Mat<_Type>& m)
{
    return m.Print(s);
}


template <typename _Type>
NN::MATH::Mat<_Type>& NN::MATH::Add (const NN::MATH::Mat<_Type>& A, const NN::MATH::Mat<_Type>& B, NN::MATH::Mat<_Type>& dest)
{
    if ((A.cols != B.cols) || (A.rows != B.rows))
    {
        throw std::runtime_error(std::format("Mat::Add expects entry of size {}x{} but got {}x{}",A.rows,A.cols,B.rows,B.cols));
    }

    if ((B.cols != B.cols) || (B.rows != B.rows))
    {
        throw std::runtime_error(std::format("Mat::Add expects destination of size {}x{} but got {}x{}",A.rows,A.cols,dest.rows,dest.cols));
    }

    for (size_t i = 0; i < A.rows*A.cols; i += 1)
    {
        dest.data.get()[i] = A.data.get()[i] + B.data.get()[i];
    }

    return dest;
}