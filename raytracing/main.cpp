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