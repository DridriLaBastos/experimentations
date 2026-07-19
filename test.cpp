#include "math/mat.hpp"

#include <iostream>

int main(int argc, char const *argv[])
{
    MATH::Mat a = MATH::Mat<>::AtRandom(3,2);
    MATH::Mat b (3,2,1.f);
    MATH::Mat id = MATH::Mat<>::Identity(2);

    MATH::Dot(a,id,b);        

    std::cout << a << "- - - - - -\n" << id << "- - - - -\n" << b << std::endl;

    return 0;
}
