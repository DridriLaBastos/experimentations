#include "math/mat.hpp"

#include <iostream>

int main(int argc, char const *argv[])
{
    MATH::Mat a = MATH::Mat<>::AtRandom(3,2);
    MATH::Mat b (3,2,1.f);
    MATH::Mat iid (2,2,{0.f,1.f,1.f,0.f});

    MATH::Dot(a,iid,b);        

    std::cout << a << "- - - - - -\n" << iid << "- - - - -\n" << b << std::endl;

    return 0;
}
