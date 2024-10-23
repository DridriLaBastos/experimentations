#include <cstdio>

#include "raytracing.hpp"

RAYTRACING_DRAW_MODULE_FUNC_DEFINITION
{
	for (size_t y = 0; y < renderingInfo->height; y += 1)
	{
		for (size_t x = 0; x < renderingInfo->width; x += 1)
		{
			const float r = x / ((float)renderingInfo->width - 1);
			const float g = y / ((float)renderingInfo->height - 1);
			const float b = 0;

			const size_t pixelBufferIndex = y * renderingInfo->width + x;
			renderingInfo->buffer[pixelBufferIndex] = Color(r,g,b);
		}
	}
}


DLL_INIT void Init(void)
{
	puts("Loading :)");
}

DLL_CLEAR void Deinit(void)
{
	puts("Unloading :(");
}

//This cannot be done in a cross-platform way
#ifdef WIN32
#include <Windows.h>
#include <Process.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason,LPVOID lpvReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			Init();
			break;
		
		case DLL_PROCESS_DETACH:
			Deinit();
			break;
		
		default:
			break;
	}

	return TRUE;
}
#endif
