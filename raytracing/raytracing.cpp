#include <cstdio>

#include "raytracing.hpp"

#include "graphics/camera.hpp"

#include "geometry/sphere.hpp"
#include "geometry/hittable_list.hpp"

#include "SFML/Graphics.hpp"

static constexpr float ASPECT_RATIO = 16.f / 9.f;
static constexpr unsigned int PIXEL_WIDTH = 400;
static constexpr unsigned int PIXEL_HEIGHT = PIXEL_WIDTH / ASPECT_RATIO;

static constexpr float VIEWPORT_WIDTH = 4.0;
static constexpr float VIEWPORT_HEIGHT = VIEWPORT_WIDTH / ASPECT_RATIO;
static constexpr float PIXEL_DX = VIEWPORT_WIDTH / PIXEL_WIDTH;
static constexpr float PIXEL_DY = VIEWPORT_HEIGHT / PIXEL_HEIGHT;

static constexpr float FOCAL_LENGTH = 4.0;

struct RenderingInfo
{
	RenderingInfo(): cam({0,0,10},5)
	{}

	sf::RenderWindow* window;
	sf::Font font;
	sf::Text text;
	HittableList world;
	Camera cam;

	std::vector<Sphere> spheres;

	unsigned int fpsCount = 0;
	sf::Time elapsed = sf::Time::Zero;
	sf::Clock clock;
};

static RenderingInfo* renderingInfo = nullptr;

#if 0
static RenderingInfo* renderingInfo = nullptr;

static float HitSphere(const Point3f& center, const float radius, const Ray& r)
{
	const Vec3f oc = center - r.Origin();
	const float a = glm::length2(r.Direction());
	const float h = glm::dot(r.Direction(),oc);
	const float c = glm::length2(oc) - radius*radius;
	const float discriminant = h*h-a*c;

	return discriminant < 0 ? -1.f : ((h - sqrtf(discriminant))/a);
}

static Color3f RayColor (const Ray& r, const HittableList& world)
{
	HitRecord rec;

	if (world.Hit(r,0,100,rec))
	{
		return .5f*Color3f(rec.normal.x+1.f,rec.normal.y+1.f,rec.normal.z+1.f);
	}

	const Vec3f unitDirection = glm::normalize(r.Direction());
	const float a = .5f*(unitDirection.y + 1.0);
	return (1.f-a)*Color3f(1.0, 1.0, 1.0) + a*Color3f(0.5, 0.7, 1.0);
}

static void Raytracing_Compute(void)
{
	for (size_t y = 0; y < PIXEL_HEIGHT; y += 1)
	{
		for (size_t x = 0; x < PIXEL_WIDTH; x += 1)
		{
			const Point3f currentPixel = renderingInfo->pixel00Loc + Vec3f{x*PIXEL_DX,-(float)y*PIXEL_DY,0};
			const Ray r (renderingInfo->cameraCenter,currentPixel-renderingInfo->cameraCenter);
			const Color3f c = RayColor(r,renderingInfo->world);
			// printf("%.3f   %.3f   %.3f\n",r.Direction().x,r.Direction().y,r.Direction().z);

			const size_t currentPixelIndex = y * PIXEL_WIDTH + x;

			renderingInfo->buffer[currentPixelIndex].red = c.r*255;
			renderingInfo->buffer[currentPixelIndex].green = c.g*255;
			renderingInfo->buffer[currentPixelIndex].blue = c.b*255;
			renderingInfo->buffer[currentPixelIndex].alpha = 255;
		}
	}
}
#endif

RAYTRACING_DRAW_MODULE_FUNC_DEFINITION
{
	sf::Event event;

	while(window.pollEvent(event))
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

	window.clear();
	renderingInfo->cam.Render(renderingInfo->world);
	renderingInfo->cam.Display(window);
	window.draw(renderingInfo->text);
	window.display();

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
}

DLL_INIT void Init(void)
{
	puts("Init");
	if (renderingInfo)
	{
		delete renderingInfo;
	}

	renderingInfo = new RenderingInfo();

	renderingInfo->font = sf::Font();
	renderingInfo->font.loadFromFile(ASSET_PATH "/fonts/Sansation.ttf");

	renderingInfo->text = sf::Text();
	renderingInfo->text.setFont(renderingInfo->font);
	renderingInfo->text.setCharacterSize(15);

	renderingInfo->spheres.emplace_back(Point3f{0,0,-2},.5f);
	renderingInfo->spheres.emplace_back(Point3f{1.5,0.5,0},1.0);
	renderingInfo->spheres.emplace_back(Point3f{-2,-1.0,-15},6.0);

	for (auto& s: renderingInfo->spheres)
	{
		renderingInfo->world.Add(&s);
	}
}

DLL_CLEAR void Deinit(void)
{
	puts("Deinit");
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
