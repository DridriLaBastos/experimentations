#include <iostream>

#include "math/mat.hpp"
#include "nn/activation_functions.hpp"

float Sigmoid (const float x)
{
    return 1.f / (1.f + std::expf(x));
}

int main(int argc, char const *argv[])
{
    MATH::Mat x (1,2,{0.f,1.f});

    MATH::Mat w1 = MATH::Mat<>::AtRandom (2,2);
    MATH::Mat b1 = MATH::Mat<>::AtRandom (1,2);
    MATH::Mat a1 (1,2);
    
    MATH::Mat w2 = MATH::Mat<>::AtRandom (2,1);
    MATH::Mat b2 = MATH::Mat<>::AtRandom (1,1);
    MATH::Mat a2 (1,1);

    MATH::Dot(x,w1,a1);
    MATH::Add(a1,b1,a1);
    MATH::ApplyFunction(NN::ActivationFunction::Sigmoid,a1);

    MATH::Dot(a1,w2,a2);
    MATH::Add(a2,b2,a2);
    MATH::ApplyFunction(NN::ActivationFunction::Sigmoid,a2);

    std::cout << a2 << std::endl;

    return 0;
}
