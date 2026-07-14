#include "math/mat.hpp"

#include <iostream>

int main(int argc, char const *argv[])
{
    NN::MATH::Mat m = NN::MATH::Mat<>::AtRandom(3,5,0,10);
    std::cout << m << std::endl;
    return 0;
}
