#ifndef NN_MATH_UTIL_HPP
#define NN_MATH_UTIL_HPP

#include <random>
#include <cstdlib>

template <typename _Type>
_Type GetRandom(const _Type min, const _Type max)
{
    static std::mt19937 engine{std::random_device{}()};

    _Type seed;

    if constexpr (std::is_integral_v<_Type>) {
        std::uniform_int_distribution<_Type> dist;
        seed = dist(engine);
    } else if constexpr (std::is_floating_point_v<_Type>) {
        std::uniform_real_distribution<_Type> dist;
        seed = dist(engine);
    } else {
        static_assert(std::is_arithmetic_v<_Type>, "Unsupported type");
        return _Type{};
    }

    return seed*(max-min) + min;
}

#endif