//
// Created by Adrien COURNAND on 17/08/2024.
//

#include <cstdio>
#include <cstdlib>

#include <chrono>

#include "raytracing.hpp"

#include "util/module/module.hpp"

int main(void)
{
#if 0
	PPMUtil ppm (IMAGE_WIDTH,IMAGE_HEIGHT);

	for (size_t y = 0; y < ppm.height; y += 1)
	{
		for (size_t x = 0; x < ppm.width; x += 1)
		{
			const float r = (float)y / (ppm.height - 1);
			const float g = (float)x / (ppm.width - 1);
			ppm.at(x,y) = Color (r,g);
		}
	}

	ppm.write("test.ppm",12);
#endif

	RAYTRACING_DRAW_MODULE_FUNC_RET(*HotReloadedRaytracingDrawModuleFuncPtr)(RAYTRACING_DRAW_MODULE_FUNC_PARAMETERS) = nullptr;
	Module raytracingDrawModule (RAYTRACING_DRAW_MODULE_FULL_PATH,RAYTRACING_DRAW_MODULE_COPY_FULL_PATH);

	raytracingDrawModule.LoadSymbol(FUNCTION_NAME(RAYTRACING_DRAW_MODULE_SYMBOL_NAME),&HotReloadedRaytracingDrawModuleFuncPtr);

	while (HotReloadedRaytracingDrawModuleFuncPtr())
	{
		if(raytracingDrawModule.ReloadIfNeeded())
		{
			raytracingDrawModule.LoadSymbol(FUNCTION_NAME(RAYTRACING_DRAW_MODULE_SYMBOL_NAME),&HotReloadedRaytracingDrawModuleFuncPtr);
		}
	}

	return EXIT_SUCCESS;
}