#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>

#include "static.hpp"

int main(void)
{
	NS::StaticFunction();
	void* dylibHandle = dlopen(DYLIB_FULL_PATH,RTLD_NOW);
	if (dylibHandle == nullptr)
	{
		perror(dlerror());
		return EXIT_FAILURE;
	}

	void (*DynamicFunctionHandle)(void) = (void(*)(void))dlsym(dylibHandle,"DynamicFunction");
	if (DynamicFunctionHandle == nullptr)
	{
		perror(dlerror());
		return EXIT_FAILURE;
	}

	DynamicFunctionHandle();
	return EXIT_SUCCESS;
}