#include <cmath>
#include <format>
#include <iostream>

/**
 * This file provides an example of using a derivative approach to make a 1 parameter neural network learn
 */

static double trainSet1[][2] = {
    {0.0, 0.0},
    {1.0, 2.0},
    {2.0, 4.0},
    {3.0, 6.0},
    {4.0, 8.0}};

/**
 * 2 parameters are used : one from th entry weight (w_0), and the other is internal to the model and is used as a bias.
 *
 */
using Model = double[2];

static double Identity(const double x)
{
    return x;
}

static double Sigmoid(const double x)
{
    return 1.0 / (1.0 + std::exp(-x));
}

static double ReLU(const double x)
{
    return (x + std::abs(x)) / 2.0;
}

static double Evaluate(const double w0, const double b, const double entry)
{
    return Identity(entry * w0 + b);
}

static double Cost(const double w0, const float b, const Model *trainingSet, const size_t trainingSetSize)
{
    double cumulativeError = 0;

    for (size_t i = 0; i < trainingSetSize; i += 1)
    {
        const double guess = Evaluate(w0, b, trainingSet[i][0]);
        const double expected = trainingSet[i][1];
        const double error = expected - guess;
        cumulativeError += error * error;
    }

    const double finalError = cumulativeError / trainingSetSize;
    return finalError;
}

static double DeriveW0(const Model &model, const double h, const Model *trainingSet, const size_t trainingSetSize)
{
    return (Cost(model[0] + h, model[1], trainingSet, trainingSetSize) - Cost(model[0], model[1], trainingSet, trainingSetSize)) / h;
}

static double DeriveB(const Model &model, const double h, const Model *trainingSet, const size_t trainingSetSize)
{
    return (Cost(model[0], model[1] + h, trainingSet, trainingSetSize) - Cost(model[0], model[1], trainingSet, trainingSetSize)) / h;
}

static void TrainingStep(Model &model, const double h, const double r, const Model *trainingSet, const size_t trainingSetSize)
{
    const double dW0 = DeriveW0(model, h, trainingSet, trainingSetSize);
    const double dB = DeriveB(model, h, trainingSet, trainingSetSize);

    model[0] -= dW0 * r;
    model[1] -= dB * r;
}

int main(void)
{
    Model m{1.977, 0.063};

    const double eps = 0.0000001;
    double e = eps * 2;
    unsigned int i = 0;
    while (e > eps)
    {
        e = Cost(m[0], m[1], trainSet1, std::size(trainSet1));
        printf("[%d] %5.5f   w0 = %.3f   b = %.3f\n", i++, e, m[0], m[1]);
        TrainingStep(m, 0.00001, 0.001, trainSet1, std::size(trainSet1));
    }

    return 0;
}
