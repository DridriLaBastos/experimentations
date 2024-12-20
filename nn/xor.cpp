#include "nn.hpp"

struct Xor {
    Matrix<2,2> w1 = Matrix<2,2>::WithRandom();
    Matrix<1,2> b1 = Matrix<1,2>::WithRandom();

    Matrix<2,2> w2 = Matrix<2,2>::WithRandom();
    Matrix<1,2> b2 = Matrix<1,2>::WithRandom();

    float Forward (const Matrix<1,2>& x)
    {
        return Sigmoidf(Sigmoidf(x*w1 + b1)*w2 + b2)[0];
    }
};

int main(int argc, char const *argv[])
{
    Xor nnXor;//xor is reserved keycode

    for (size_t i = 0; i < 2; i += 1)
    {
        for (size_t j = 0; j < 2; j += 1)
        {
            printf("%ld ^ %ld = %.3f\n",i,j,nnXor.Forward(Vector{(float)i,(float)j}));
        }
    }
    return 0;
}

