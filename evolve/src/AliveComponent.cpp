#include <random>

#include "scene/AliveComponent.hpp"

static std::random_device rd;
static std::mt19937 gen(rd());

static constexpr float PEAK_VALUE = 0.4f;
static constexpr float DEVIATION = 0.25f;

static const float LOG_NORMAL_MEAN = std::log(PEAK_VALUE) + (DEVIATION * DEVIATION) / 2.0f;
static std::lognormal_distribution<float> distribution (LOG_NORMAL_MEAN,DEVIATION);

AliveComponent::AliveComponent()
	: radius(distribution(gen))
{
}