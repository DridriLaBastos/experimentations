#include <random>
#include <thread>
#include <iostream>

#include "scene/Scene.hpp"
#include "scene/AliveComponent.hpp"
#include "scene/SpriteComponent.hpp"

#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"
#include "FastNoise/FastNoise.h"
#include "scene/PhysicComponent.hpp"

static std::random_device rd;
static std::mt19937 gen(rd());

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;

#define RESOURCE_PATH(path) RESOURCE_FOLDER "/" path

using Clock = std::chrono::high_resolution_clock;
using MilliSeconds = std::chrono::duration<float, std::milli>;
using Seconds      = std::chrono::duration<float>;

static constexpr float TARGET_FPS = 60.0;
static constexpr float TARGET_TPS = 100.0;
static constexpr float PIXEL_PER_METER = 50.0;
static constexpr unsigned int PHYSIC_SOLVER_SUB_STEPS = 6;

static constexpr Seconds SEC_PER_FRAME (1.0f/TARGET_FPS);
static constexpr Seconds SEC_PER_TICK (1.0/TARGET_TPS);
static constexpr Seconds GAME_LOOP_SLEEP = std::min(SEC_PER_FRAME,SEC_PER_TICK);

static std::uniform_real_distribution<float> distributionX(0,WINDOW_WIDTH);
static std::uniform_real_distribution<float> distributionY(0,WINDOW_HEIGHT);

float MapValue(float n, float min, float max, float a, float b) {
    return a + ((n - min) * (b - a)) / (max - min);
}

int main(int argc, char const *argv[])
{
	sf::RenderWindow window(sf::VideoMode({720,480}),"Evolve");

	sf::View mainCamera = window.getDefaultView();

	sf::Font font (RESOURCE_PATH("fonts/LTKaraoke-SemiBold.ttf"));
	sf::Text text (font);
	text.setPosition({10,10});
	text.setCharacterSize(15);
	text.setFillColor(sf::Color::Red);

	sf::Texture particleTexture (RESOURCE_PATH("textures/particle.png"));

	auto frameTimeBegin = Clock::now();
	auto frameTimeEnd = Clock::now();
	unsigned int frameCount = 0;
	unsigned int tickCount = 0;
	Seconds timeSinceLastFrame (0);
	Seconds timeSinceLastUpdate (0);
	Seconds elapsedTime (0);

	float zoomAccelerationFactor = 300.0;
	float zoomAcceleration = 0;
	float zoomVelocity = 0.0;
	float maxZoomVelocity = 500.0;

	float cameraAccelerationFactor = 300.0;
	sf::Vector2f cameraAcceleration;
	sf::Vector2f cameraVelocity;

	b2Version version = b2GetVersion();
	printf("*** BOX2D Version : %d.%d.%d ***\n",version.major,version.minor,version.revision);

	b2WorldDef wordlDef = b2DefaultWorldDef();
	wordlDef.gravity = { 0.f,  0.f };
	b2WorldId worldId = b2CreateWorld(&wordlDef);

	std::vector<float> noiseOutput (100.0*100.0);
	auto node = FastNoise::New<FastNoise::Perlin>();
	auto output = node->GenUniformGrid2D(noiseOutput.data(),0,0,100,100,0.01,19061996);

	std::vector<sf::Color> colorNoiseOutput (noiseOutput.size());

	for (size_t i = 0; i < noiseOutput.size(); i += 1)
	{
		const float n = noiseOutput[i];
		sf::Color groundColor = sf::Color::Blue;

		if (n >= 0.9) {
			groundColor = sf::Color::White;
		}
		else if (n >= 0.7) {
			groundColor = sf::Color::Black;
		}
		else if (n >= -0.1) {
			groundColor = sf::Color::Green;
		}
		else if (n >= -0.4) {
			groundColor = sf::Color::Yellow;
		}
		const uint8_t colorValue = MapValue(n,output.min,output.max,0,255);
		colorNoiseOutput[i] = groundColor;
	}

	sf::Texture groundTexture (sf::Vector2u{100,100});
	groundTexture.update((const uint8_t*)colorNoiseOutput.data());

	Scene s;

	for (size_t i = 0; i < 1000; i += 1)
	{
		SceneEntity e = s.SpawnEntity();

		AliveComponent& aliveComponent = e.AddComponent<AliveComponent>();

		const float x = distributionX(gen);
		const float y = distributionY(gen);

		PhysicComponent& physicComponent = e.AddComponent<PhysicComponent>(worldId,aliveComponent.radius,b2Vec2{x/PIXEL_PER_METER,y/PIXEL_PER_METER});
		b2Body_SetLinearVelocity(physicComponent.bodyId,{5*aliveComponent.radius,0});

		const float finalPixelSize = aliveComponent.radius * PIXEL_PER_METER;
		const float scaleFactor = finalPixelSize / particleTexture.getSize().x;
		SpriteComponent& spriteComponent = e.AddComponent<SpriteComponent>(particleTexture);
		spriteComponent.sprite.setOrigin({particleTexture.getSize().x/2.f,particleTexture.getSize().y/2.f});
		spriteComponent.sprite.setScale({scaleFactor,scaleFactor});
	}

	auto groundEntity = s.SpawnEntity();
	auto& groundSpriteComponent = groundEntity.AddComponent<SpriteComponent>(groundTexture);
	groundSpriteComponent.sprite.setScale({PIXEL_PER_METER,PIXEL_PER_METER});

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			else if (auto* key = event->getIf<sf::Event::KeyPressed>())
			{
				float newZoomAcceleration = 0;
				sf::Vector2f newCameraAcceleration;
				if (key->code == sf::Keyboard::Key::A)
				{
					newZoomAcceleration += zoomAccelerationFactor;
				}

				if (key->code == sf::Keyboard::Key::E)
				{
					newZoomAcceleration -= zoomAccelerationFactor;
				}

				if (key->code == sf::Keyboard::Key::Z)
				{
					newCameraAcceleration.y -= cameraAccelerationFactor;
				}

				if (key->code == sf::Keyboard::Key::S)
				{
					newCameraAcceleration.y += cameraAccelerationFactor;
				}

				if (key->code == sf::Keyboard::Key::D)
				{
					newCameraAcceleration.x += cameraAccelerationFactor;
				}

				if (key->code == sf::Keyboard::Key::Q)
				{
					newCameraAcceleration.x -= cameraAccelerationFactor;
				}

				zoomAcceleration = newZoomAcceleration;
				cameraAcceleration = newCameraAcceleration;
			}
			else if (auto* key = event->getIf<sf::Event::KeyReleased>())
			{
				switch (key->code)
				{
					case sf::Keyboard::Key::A:
					case sf::Keyboard::Key::E:
						zoomAcceleration = 0;
					break;

					case sf::Keyboard::Key::Z:
					case sf::Keyboard::Key::S:
						cameraAcceleration.y = 0;
					break;

					case sf::Keyboard::Key::Q:
					case sf::Keyboard::Key::D:
						cameraAcceleration.x = 0;
					break;

					default:
						break;
				}
			}
		}

		while (timeSinceLastUpdate >= SEC_PER_TICK)
		{
			tickCount += 1;
			sf::Vector2f mainCameraSize = mainCamera.getSize();
			sf::Vector2f newCameraSize = mainCameraSize;
			
			zoomVelocity += zoomAcceleration * SEC_PER_FRAME.count();
			newCameraSize.x += zoomVelocity * SEC_PER_FRAME.count();
			newCameraSize.y += zoomVelocity * SEC_PER_FRAME.count();
			mainCamera.zoom(newCameraSize.x/mainCameraSize.x);

			//If the sign are opposed, then the acceleration is against the movement and we want to attenuate the zoom
			const bool accelerationOpposed = (zoomAcceleration / zoomVelocity) < 0;
			const bool notAccelerating = std::abs(zoomAcceleration) < 0.01;
			const bool attenuateZoom = accelerationOpposed || notAccelerating;

			if (attenuateZoom)
				zoomVelocity += -zoomVelocity*5*SEC_PER_FRAME.count();

			cameraVelocity += cameraAcceleration*SEC_PER_FRAME.count();
			mainCamera.move(cameraVelocity*SEC_PER_FRAME.count());

			if (cameraAcceleration.lengthSquared() <= cameraAccelerationFactor)
				cameraVelocity += -cameraVelocity * 5.f * SEC_PER_FRAME.count();

			auto aliveEntities = s.mRegistry.view<AliveComponent,PhysicComponent>();

			for (auto& aliveEntity : aliveEntities)
			{
				auto [aliveComponent,physicComponent] = aliveEntities.get<AliveComponent,PhysicComponent>(aliveEntity);
				b2Body_ApplyTorque(physicComponent.bodyId,aliveComponent.radius,true);
			}

			b2World_Step(worldId,SEC_PER_TICK.count(),4);
			timeSinceLastUpdate -= SEC_PER_TICK;
		}

		while (timeSinceLastFrame >= SEC_PER_FRAME)
		{
			frameCount += 1;

			window.clear(sf::Color::Magenta);
			window.setView(mainCamera);

			//TODO: Optimize access to Box2D simulation objects using the thing that I forget but you know what I mean
			//Update the rendering components that have a physic simulation attached to it
			auto physicSpriteView = s.mRegistry.view<SpriteComponent, PhysicComponent>();
			for (auto& entity : physicSpriteView)
			{
				auto [spriteComponent,physicComponent] = physicSpriteView.get<SpriteComponent,PhysicComponent>(entity);

				b2Vec2 position = b2Body_GetPosition(physicComponent.bodyId);
				b2Rot rotation  = b2Body_GetRotation(physicComponent.bodyId);

				//In SFML Y axis is pointing down and 0,0 is in the top left corner
				//In SFML rotation is toward the Y axis wich is pointing down
				//TODO: This should be SIMD-able
				rotation.s *= -1;
				position.y = window.getSize().y - position.y;

				spriteComponent.sprite.setPosition({position.x,position.y});
				spriteComponent.sprite.setRotation( sf::radians(b2Rot_GetAngle(rotation)));
			}

			//TODO: Can I make an optimization here by querying only the components not in the preview view
			//		and grouping them in a adjacent structures ?
			auto spriteEntities = s.mRegistry.view<SpriteComponent>();
			unsigned int i = 0;
			for (auto spriteEntity : spriteEntities)
			{
				SpriteComponent& spriteComponent = s.mRegistry.get<SpriteComponent>(spriteEntity);
				window.draw(static_cast<sf::Sprite>(spriteComponent));
			}

			window.setView(window.getDefaultView());
			window.draw(text);
			window.display();

			timeSinceLastFrame -= SEC_PER_FRAME;
		}

		if (elapsedTime > Seconds(1))
		{
			char buffer [64];
			const float averageFrameTime = elapsedTime.count() / static_cast<float>(frameCount);
			snprintf(buffer,sizeof(buffer),"FPS %d (%.3fms)\nGame tick : %d", frameCount, averageFrameTime*1000.0f,tickCount);
			text.setString(buffer);

			frameCount = 0;
			tickCount = 0;
			elapsedTime -= Seconds(1);
		}

		frameTimeEnd = Clock::now();
		MilliSeconds loopTime = frameTimeEnd - frameTimeBegin;
		timeSinceLastFrame += loopTime;
		timeSinceLastUpdate += loopTime;
		elapsedTime += loopTime;
		frameTimeBegin = frameTimeEnd;

		std::this_thread::sleep_for(GAME_LOOP_SLEEP);
	}

	b2DestroyWorld(worldId);
	return 0;
}
