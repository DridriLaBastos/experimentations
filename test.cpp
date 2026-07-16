#include "math/mat.hpp"

#include <iostream>

int main(int argc, char const *argv[])
{
    NN::MATH::Mat a (2,2,1);
    NN::MATH::Mat b (2,2,1);

    NN::MATH::Add(a,b,a);

    std::cout << a << "- - - - - -\n" << b << "- - - - -\n" << a << std::endl;

    return 0;
}
