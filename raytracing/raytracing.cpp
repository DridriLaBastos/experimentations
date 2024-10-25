#include <cstdio>

#include "color.hpp"
#include "raytracing.hpp"

#include "SFML/Graphics.hpp"

static constexpr unsigned int PIXEL_WIDTH = 200;
static constexpr unsigned int PIXEL_HEIGHT = 200;

struct RenderingInfo
{
	sf::RenderWindow* window;
	sf::Texture* texture;
	sf::Sprite* sprite;
	Color* buffer;
	sf::Font font;
	sf::Text text;

	unsigned int fpsCount = 0;
	sf::Time elapsed = sf::Time::Zero;
	sf::Clock clock;

	void Free(void)
	{
		delete window;
		delete texture;
		delete sprite;
		delete[] buffer;
	}
};

static RenderingInfo* renderingInfo = nullptr;

static void Raytracing_Compute(void)
{
	for (size_t y = 0; y < PIXEL_HEIGHT; y += 1)
	{
		for (size_t x = 0; x < PIXEL_WIDTH; x += 1)
		{
			const size_t currentPixelIndex = y * PIXEL_WIDTH + x;
			const float r = x / (float)(PIXEL_WIDTH - 1);
			const float g = y / (float)(PIXEL_HEIGHT - 1);
			const float b = 0.0f;

			renderingInfo->buffer[currentPixelIndex] = Color(r,g,b);
		}
	}
}

RAYTRACING_DRAW_MODULE_FUNC_DEFINITION
{
	// for (size_t y = 0; y < renderingInfo->height; y += 1)
	// {
	// 	for (size_t x = 0; x < renderingInfo->width; x += 1)
	// 	{
	// 		const float r = x / ((float)renderingInfo->width - 1);
	// 		const float g = y / ((float)renderingInfo->height - 1);
	// 		const float b = 0;

	// 		const size_t pixelBufferIndex = y * renderingInfo->width + x;
	// 		renderingInfo->buffer[pixelBufferIndex] = Color(r,g,b);
	// 	}
	// }

	sf::Event event;

	while(renderingInfo->window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			renderingInfo->window->close();
			break;
		
		default:
			break;
		}
	}

	renderingInfo->window->clear();
	Raytracing_Compute();
	renderingInfo->texture->update((sf::Uint8*)renderingInfo->buffer);
	renderingInfo->window->draw(*renderingInfo->sprite);
	renderingInfo->window->draw(renderingInfo->text);
	renderingInfo->window->display();

	renderingInfo->fpsCount += 1;
	renderingInfo->elapsed += renderingInfo->clock.restart();

	if (renderingInfo->elapsed >= sf::seconds(1))
	{
		char buffer[64];
		snprintf(buffer,sizeof(buffer),"FPS : %d (%dms)\n",renderingInfo->fpsCount, renderingInfo->elapsed.asMilliseconds() / renderingInfo->fpsCount);
		renderingInfo->text.setString(buffer);
		renderingInfo->fpsCount = 0;
		renderingInfo->elapsed = sf::Time::Zero;
	}

	return renderingInfo->window->isOpen();
}

DLL_INIT void Init(void)
{
	puts("Init");
	if (renderingInfo)
	{
		delete renderingInfo;
	}

	renderingInfo = new RenderingInfo();

	renderingInfo->buffer = new Color[PIXEL_HEIGHT*PIXEL_WIDTH];

	renderingInfo->window = new sf::RenderWindow(sf::VideoMode(640,480),"Raytracing");
	renderingInfo->window->setFramerateLimit(30);

	renderingInfo->texture = new sf::Texture();
	renderingInfo->texture->create(PIXEL_WIDTH,PIXEL_HEIGHT);

	renderingInfo->sprite = new sf::Sprite(*renderingInfo->texture);

	renderingInfo->font = sf::Font();
	renderingInfo->font.loadFromFile(ASSET_PATH "/fonts/Sansation.ttf");

	renderingInfo->text = sf::Text();
	renderingInfo->text.setFont(renderingInfo->font);
	renderingInfo->text.setCharacterSize(15);
}

DLL_CLEAR void Deinit(void)
{
	puts("Deinit");
	if (renderingInfo)
	{
		renderingInfo->Free();
	}
	delete renderingInfo;
}

//This cannot be done in an cross platform way
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
