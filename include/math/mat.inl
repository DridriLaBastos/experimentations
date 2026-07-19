#include <format>
#include <stdexcept>

#include "math/mat.hpp"
#include "math/util.hpp"

template<typename _Type>
MATH::Mat<_Type>::Mat(const size_t r, const size_t c, _Type* initData)
    : managedData{initData ? nullptr : std::make_unique<_Type[]>(r * c)}, rows{r}, cols{c}, data{initData ? initData : managedData.get()}
{
    //TODO: Maybe throw of allocation fails ?
}

template<typename _Type>
MATH::Mat<_Type>::Mat (const size_t r, const size_t c, const _Type x): Mat<_Type>(r,c)
{
    for (size_t i = 0; i < r*c; i+= 1)
    {
        this->data[i] = x;
    }
}

template <typename _Type>
MATH::Mat<_Type>::Mat (const size_t r, const size_t c, std::initializer_list<_Type> initData)
    : managedData{nullptr}, rows{r}, cols{c}, data{nullptr}
{
    if (r == 0 || c == 0) {
        throw std::runtime_error("Mat dimensions must be greater than zero");
    }

    if (initData.size() != r * c) {
        throw std::runtime_error(std::format("Expected {} elements in initializer list, got {}", r * c, initData.size()));
    }

    data = const_cast<_Type*>(initData.begin());
}

template <typename _Type>
MATH::Mat<_Type> MATH::Mat<_Type>::AtRandom (const size_t r, const size_t c, const _Type min, const _Type max)
{
    MATH::Mat<_Type> m (r,c);

    //TODO: We probably want to have iterators here
    for (size_t i = 0; i < r*c; i += 1) {
        m.data[i] = GetRandom<_Type>(min,max);
    }

    return m;
}

template <typename _Type>
MATH::Mat<_Type> MATH::Mat<_Type>::Identity (const size_t n)
{
    Mat<_Type> m (n,n,0);

    for (size_t i = 0; i < n; i += 1)
    {
        m.data[i*n + i] = 1;
    }

    return m;
}

template <typename _Type>
std::ostream& MATH::Mat<_Type>::Print(std::ostream& s) const
{
    s << "[\n";
    for (size_t r = 0; r < this->rows; r += 1)
    {
        s << "\t";
        for (size_t c = 0; c < this->cols; c += 1) {
            s << std::format("{:4} ", this->At(r,c));
        }
        s << std::endl;
    }

    s << "]";

    return s;
}

template <typename _Type>
std::ostream& operator<< (std::ostream& s, const MATH::Mat<_Type>& m)
{
    return m.Print(s);
}


template <typename _Type>
MATH::Mat<_Type>& MATH::Add (const MATH::Mat<_Type>& A, const MATH::Mat<_Type>& B, MATH::Mat<_Type>& dest)
{
    if ((A.cols != B.cols) || (A.rows != B.rows))
    {
        throw std::runtime_error(std::format("Mat::Add expects entry of dimensions ({},{}) but got ({},{})",A.rows,A.cols,B.rows,B.cols));
    }

    if ((B.cols != B.cols) || (B.rows != B.rows))
    {
        throw std::runtime_error(std::format("Mat::Add expects destination of dimensions ({},{}) but got ({},{})",A.rows,A.cols,dest.rows,dest.cols));
    }

    for (size_t i = 0; i < A.rows*A.cols; i += 1)
    {
        dest.data[i] = A.data[i] + B.data[i];
    }

    return dest;
}

template <typename _Type>
MATH::Mat<_Type>& MATH::Dot (const MATH::Mat<_Type>& A, const MATH::Mat<_Type>& B, MATH::Mat<_Type>& dest)
{
    // 1x2 2x3 -> 1x3
    if (A.cols != B.rows)
    {
        throw std::runtime_error(std::format("Mat::Dot expects entry of dimensions (_,{}) ({},_) but got (_,{}) ({},_)",A.cols,A.cols,A.cols,B.rows));
    }

    if ((dest.rows != A.rows) || (dest.cols != B.cols))
    {
        throw std::runtime_error(std::format("Mat::Dot expects dest of dimensions ({},{}) but got ({},{})",A.rows,B.cols,dest.rows,dest.cols));
    }

    const size_t n = A.cols;

    for (size_t i = 0; i < dest.rows; i += 1)
    {
        for (size_t j = 0; j < dest.cols; j += 1)
        {
            dest.At(i,j) = 0;
            for (size_t k = 0; k < n; k += 1)
            {
                dest.At(i,j) += A.At(i,k) * B.At(k,j);
            }
        }
    }

    return dest;
}

template <typename _Type>
MATH::Mat<_Type>& ApplyFunction (const std::function<_Type(_Type)> func, MATH::Mat<_Type>& M)
{
    for (size_t i = 0; i < M.cols * M.rows; i += 1)
    {
        M.data[i] = func(M.data[i]);
    }

    return M;
}
