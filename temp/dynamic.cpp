#include <cstdio>

#include "static.hpp"

extern "C" __attribute__((visibility("default"))) void DynamicFunction(void)
{
	puts("Dynamic Function");
	NS::StaticFunction();
}
