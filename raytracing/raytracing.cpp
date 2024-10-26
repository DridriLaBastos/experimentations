#include <cstdio>

#include "ray.hpp"
#include "color.hpp"
#include "vec3f.hpp"
#include "raytracing.hpp"

#include "SFML/Graphics.hpp"

static constexpr float ASPECT_RATIO = 16.f / 9.f;
static constexpr unsigned int PIXEL_WIDTH = 400;
static constexpr unsigned int PIXEL_HEIGHT = PIXEL_WIDTH / ASPECT_RATIO;

static constexpr float VIEWPORT_WIDTH = 2.0;
static constexpr float VIEWPORT_HEIGHT = 1.0;//VIEWPORT_WIDTH / ASPECT_RATIO;
static constexpr float PIXEL_DX = VIEWPORT_WIDTH / PIXEL_WIDTH;
static constexpr float PIXEL_DY = VIEWPORT_HEIGHT / PIXEL_HEIGHT;

static constexpr float FOCAL_LENGTH = 1.0;

struct RenderingInfo
{
	sf::RenderWindow* window;
	sf::Texture* texture;
	sf::Sprite* sprite;
	RenderColor* buffer;
	sf::Font font;
	sf::Text text;

	unsigned int fpsCount = 0;
	sf::Time elapsed = sf::Time::Zero;
	sf::Clock clock;

	Point3f cameraCenter {.0f,.0f,.0f};
	Point3f viewportCenter {.0f,.0f, FOCAL_LENGTH };
	Point3f viewportUpperLeft = viewportCenter + Point3f(-VIEWPORT_WIDTH/2.f,VIEWPORT_HEIGHT/2.f);
	Point3f pixel00Loc = viewportUpperLeft;// + Point3f{ PIXEL_DX/2.0,-PIXEL_DY/2.0,0 };

	void Free(void)
	{
		delete window;
		delete texture;
		delete sprite;
		delete[] buffer;
	}
};

static RenderingInfo* renderingInfo = nullptr;

static Color3f RayColor (const Ray& r)
{
	const Vec3f unitDirection = Vec3f::Normalize(r.Direction());
	const float a = .5f*(unitDirection.y + 1.0);
	// printf("%.3f\n",a);
	return (1.0-a)*Color3f(1.0, 1.0, 1.0) + a*Color3f(0.5, 0.7, 1.0);
}

static void Raytracing_Compute(void)
{
	for (size_t y = 0; y < PIXEL_HEIGHT; y += 1)
	{
		for (size_t x = 0; x < PIXEL_WIDTH; x += 1)
		{
			const Point3f currentPixel = renderingInfo->pixel00Loc + Vec3f{x*PIXEL_DX,-(float)y*PIXEL_DY,0};
			const Ray r (renderingInfo->cameraCenter,currentPixel-renderingInfo->cameraCenter);
			const Color3f c = RayColor(r);
			// printf("%.3f   %.3f   %.3f\n",r.Direction().x,r.Direction().y,r.Direction().z);

			const size_t currentPixelIndex = y * PIXEL_WIDTH + x;

			renderingInfo->buffer[currentPixelIndex].red = c.r*255;
			renderingInfo->buffer[currentPixelIndex].green = c.g*255;
			renderingInfo->buffer[currentPixelIndex].blue = c.b*255;
			renderingInfo->buffer[currentPixelIndex].alpha = 255;
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

	renderingInfo->buffer = new RenderColor[PIXEL_HEIGHT*PIXEL_WIDTH];

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
