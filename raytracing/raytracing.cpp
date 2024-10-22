#include "raytracing.hpp"

RAYTRACING_DRAW_MODULE_FUNC_DEFINITION
{
	for (size_t i = 0; i < (renderingInfo->height)*(renderingInfo->width); i += 1)
	{
		renderingInfo->buffer[i] = Colors::YELLOW;
	}
}

#include <cstdio>

DLL_INIT void Inti(void)
{
	puts("Loading :)");
}

DLL_CLEAR void Deinit(void)
{
	puts("Unloading :(");
}
