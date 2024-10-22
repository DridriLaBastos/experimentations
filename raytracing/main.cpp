//
// Created by Adrien COURNAND on 17/08/2024.
//

#include <cstdio>
#include <cstdlib>

#include "SFML/Graphics.hpp"

#include "ppm.hpp"
#include "color.hpp"
#include "raytracing.hpp"

#include "util/module/module.hpp"

static constexpr unsigned int PIXEL_HEIGHT = 200;
static constexpr unsigned int PIXEL_WIDTH  = 200;
static Color buffer [PIXEL_HEIGHT * PIXEL_WIDTH];

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

	Raytracing::RenderingInfo renderingInfo;

	RAYTRACING_DRAW_MODULE_FUNC_RET(*HotReloadedRaytracingDrawModuleFuncPtr)(RAYTRACING_DRAW_MODULE_FUNC_PARAMETERS) = nullptr;
	Module raytracingDrawModule (RAYTRACING_DRAW_MODULE_FULL_PATH,RAYTRACING_DRAW_MODULE_COPY_FULL_PATH);

	raytracingDrawModule.LoadSymbol(FUNCTION_NAME(RAYTRACING_DRAW_MODULE_SYMBOL_NAME),&HotReloadedRaytracingDrawModuleFuncPtr);

	sf::RenderWindow window (sf::VideoMode(640,480),"Raytracing");
	window.setFramerateLimit(30);
	
	while (window.isOpen())
	{
		sf::Event event;
		
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				
				default:
					break;
			}
		}

		if(raytracingDrawModule.ReloadIfNeeded())
		{
			raytracingDrawModule.LoadSymbol(FUNCTION_NAME(RAYTRACING_DRAW_MODULE_SYMBOL_NAME),&HotReloadedRaytracingDrawModuleFuncPtr);
		}
		window.clear();
		HotReloadedRaytracingDrawModuleFuncPtr(&renderingInfo);
		window.display();
	}

	return EXIT_SUCCESS;
}