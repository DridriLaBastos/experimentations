//
// Created by Adrien COURNAND on 17/08/2024.
//

#include <cstdio>
#include <cstdlib>

#include <chrono>

#include "SFML/Graphics.hpp"

#include "raytracing.hpp"

#include "util/module/module.hpp"

static constexpr unsigned int PIXEL_HEIGHT = 200;
static constexpr unsigned int PIXEL_WIDTH  = 200;
static Color buffer [PIXEL_HEIGHT * PIXEL_WIDTH];

using Clock = std::chrono::high_resolution_clock;

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
	renderingInfo.buffer = buffer;
	renderingInfo.height = PIXEL_HEIGHT;
	renderingInfo.width  = PIXEL_WIDTH;

	RAYTRACING_DRAW_MODULE_FUNC_RET(*HotReloadedRaytracingDrawModuleFuncPtr)(RAYTRACING_DRAW_MODULE_FUNC_PARAMETERS) = nullptr;
	Module raytracingDrawModule (RAYTRACING_DRAW_MODULE_FULL_PATH,RAYTRACING_DRAW_MODULE_COPY_FULL_PATH);

	raytracingDrawModule.LoadSymbol(FUNCTION_NAME(RAYTRACING_DRAW_MODULE_SYMBOL_NAME),&HotReloadedRaytracingDrawModuleFuncPtr);

	sf::RenderWindow window (sf::VideoMode(640,480),"Raytracing");
	window.setFramerateLimit(30);

	sf::Texture texture;
	texture.create(PIXEL_WIDTH,PIXEL_HEIGHT);

	sf::Sprite sprite;
	sprite.setTexture(texture);

	sf::Font font;
	font.loadFromFile(ASSET_PATH "/fonts/Sansation.ttf");
	sf::Text text;
	text.setFont(font);

	sf::Clock clock;
	sf::Time accumulatedRenderTime = sf::Time::Zero;
	std::chrono::nanoseconds averageRtTime;
	unsigned int fps = 0;

	while (window.isOpen())
	{
		accumulatedRenderTime += clock.restart();
		fps += 1;
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
		const std::chrono::time_point begin = Clock::now();
		HotReloadedRaytracingDrawModuleFuncPtr(&renderingInfo);
		averageRtTime += Clock::now() - begin;
		texture.update((sf::Uint8*)renderingInfo.buffer);
		window.draw(sprite);
		window.draw(text);
		window.display();

		if (accumulatedRenderTime >= sf::seconds(1))
		{
			char buffer[256];

			snprintf(buffer,sizeof(buffer),"FPS %d (%3dms)\nRT %3d ms",
				fps,accumulatedRenderTime.asMilliseconds() / fps,std::chrono::duration_cast<std::chrono::milliseconds>(averageRtTime).count() / fps);
			fps = 0;
			accumulatedRenderTime = sf::Time::Zero;
			averageRtTime = std::chrono::nanoseconds(0);
			text.setString(buffer);
		}
	}

	return EXIT_SUCCESS;
}