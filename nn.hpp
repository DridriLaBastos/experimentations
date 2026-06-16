#ifndef NN_HPP
#define NN_HPP

#include <cmath>
#include <vector>
#include <cstdlib>
#include <iostream>

template <typename T>
struct Mallocator
{
	using value_type = T;
    Mallocator() = default;
 
    template<class U>
    constexpr Mallocator(const Mallocator <U>&) noexcept {}
 
    T* allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_array_new_length();
		
		auto p = static_cast<T*>(std::malloc(n * sizeof(T)));

		if (!p)
			throw std::bad_alloc();
 
		#ifdef MALLOCATOR_REPORT
            report(p, n);
		#endif
            return p;
    }
 
    void deallocate(T* p, std::size_t n) noexcept
    {
	#ifdef MALLOCATOR_REPORT
        report(p, n, 0);
	#endif
        std::free(p);
    }
private:
    void report(T* p, std::size_t n, bool alloc = true) const
    {
        std::cout << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n
                  << " bytes at " << std::hex << std::showbase
                  << reinterpret_cast<void*>(p) << std::dec << '\n';
    }
};

using Vector = std::vector<float,Mallocator<float> >;

template <size_t Row, size_t Column>
class Matrix
{
    public:
        Matrix(const Vector& init = {});

		void Print(void) const;

public:
        void operator+= (const Matrix& m);
		float& operator[] (const size_t i);
		const float& operator[] (const size_t i) const;

public:
		static constexpr size_t SIZE = Row*Column;
	
	public:
		static Matrix<Row,Column> WithRandom(const float min = .0f, const float max = 1.f);
		static Matrix<Row,Column> WithValue(const float x = 0.0);
    
    private:
		Vector mWeights;
};

template <size_t L, size_t M, size_t N>
Matrix <L,N> operator* (const Matrix<L,M>& A, const Matrix<M,N>& B);

template <size_t L, size_t M>
Matrix<L,M> operator+(const Matrix<L,M>& A, const Matrix<L,M>& B);

float Sigmoidf(const float x, const float a = 1.0)
{
    return a/(1.f + std::exp(-x));
}

template<size_t L, size_t M>
Matrix<L,M>&& Sigmoidf(Matrix<L,M>&& m, const float a = 1.0)
{
    for (size_t i = 0; i < L*M; i+=1)
    {
        m[i] = Sigmoidf(m[i],a);
    }
    return std::move(m);
}

#include "matrix.cpp"

#endif