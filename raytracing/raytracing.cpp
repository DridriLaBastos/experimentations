#include "raytracing.hpp"

static constexpr unsigned int PIXEL_HEIGHT = 200;
static constexpr unsigned int PIXEL_WIDTH  = 200;
static Color buffer [PIXEL_HEIGHT * PIXEL_WIDTH];

RAYTRACING_DRAW_MODULE_FUNC_DEFINITION
{
	outRenderingInfo->buffer = buffer;
	outRenderingInfo->height = PIXEL_HEIGHT;
	outRenderingInfo->width = PIXEL_WIDTH;
	for (size_t i = 0; i < (PIXEL_HEIGHT)*(PIXEL_WIDTH); i += 1)
	{
		outRenderingInfo->buffer[i] = Colors::MAGENTA;
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
