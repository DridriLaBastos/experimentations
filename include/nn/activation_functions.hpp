#ifndef NN_ACTIVATION_FUNCTION_HPP
#define NN_ACTIVATION_FUNCTION_HPP

#include <cmath>

namespace NN::ActivationFunction
{
    float Sigmoid (const float x)
    {
        return 1.f / (1.f + std::expf(x));
    }
}

#endif
