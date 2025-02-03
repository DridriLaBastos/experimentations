#include <thread>
#include <iostream>

#include "scene/Scene.hpp"
#include "scene/SpriteComponent.hpp"

#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"
#include "FastNoise/FastNoise.h"

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;

#define RESOURCE_PATH(path) RESOURCE_FOLDER "/" path

using Clock = std::chrono::high_resolution_clock;
using MilliSeconds = std::chrono::duration<float, std::milli>;
using Seconds      = std::chrono::duration<float>;

static constexpr float TARGET_FPS = 30.0;
static constexpr float TARGET_TPS = 45.0;
static constexpr float PIXEL_PER_METER = 10.0;
static constexpr unsigned int PHYSIC_SOLVER_SUB_STEPS = 6;

static constexpr Seconds SEC_PER_FRAME (1.0f/TARGET_FPS);
static constexpr Seconds SEC_PER_TICK (1.0/TARGET_TPS);
static constexpr Seconds GAME_LOOP_SLEEP = std::min(SEC_PER_FRAME,SEC_PER_TICK);

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

	sf::Texture texture (RESOURCE_PATH("textures/particle.png"));
	sf::Sprite sprite(texture);

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

	//I apply the pixel per mettre when defining the forces so that I don't have to multiply all the value given by 
	// Box2d when setting the position of the sprite.
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = { sprite.getPosition().x, sprite.getPosition().y };
	bodyDef.linearVelocity = { .0f, PIXEL_PER_METER };
	b2BodyId bodyId = b2CreateBody(worldId,&bodyDef);

	b2Polygon dynamicCircle = b2MakeRoundedBox(1.0,1.0,1.0);
	b2ShapeDef defaultShapeDef = b2DefaultShapeDef();
	defaultShapeDef.density = 1.0;
	defaultShapeDef.friction = 0;
	b2ShapeId shapeId = b2CreatePolygonShape(bodyId,&defaultShapeDef,&dynamicCircle);

	std::vector<float> noiseOutput (1000.0*1000.0);
	auto node = FastNoise::New<FastNoise::Perlin>();
	auto output = node->GenUniformGrid2D(noiseOutput.data(),0,0,1000,1000,0.01,19061996);

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

	sf::Texture groundTexture (sf::Vector2u{1000,1000});
	groundTexture.update((const uint8_t*)colorNoiseOutput.data());
	sf::Sprite groundSprite (groundTexture);
	groundSprite.setScale({PIXEL_PER_METER,PIXEL_PER_METER});

	Scene s;
	auto s1 = s.Add<SpriteComponent>(SpriteComponent(texture));
	auto s2 = s.Add<SpriteComponent>(SpriteComponent(texture));

	s.mRegistry.get<SpriteComponent>(s1).mSprite.setPosition({300,300});

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

		while (timeSinceLastFrame >= SEC_PER_FRAME)
		{
			frameCount += 1;

			window.clear(sf::Color::Magenta);
			window.setView(mainCamera);

			auto spriteEntities = s.mRegistry.view<SpriteComponent>();
			unsigned int i = 0;
			for (auto spriteEntity : spriteEntities)
			{
				SpriteComponent& component = s.mRegistry.get<SpriteComponent>(spriteEntity);
				printf("%d  %.3f %.3f\n",i++,component.mSprite.getPosition().x,component.mSprite.getPosition().x);
				window.draw(s.mRegistry.get<SpriteComponent>(spriteEntity).mSprite);
			}

			window.setView(window.getDefaultView());
			window.draw(text);
			window.display();

			timeSinceLastFrame -= SEC_PER_FRAME;
		}

		while (timeSinceLastUpdate >= SEC_PER_TICK)
		{
			tickCount += 1;
			sf::Vector2f mainCameraSize = mainCamera.getSize();
			zoomVelocity += zoomAcceleration * SEC_PER_FRAME.count();
			mainCameraSize.x += zoomVelocity * SEC_PER_FRAME.count();
			mainCameraSize.y += zoomVelocity * SEC_PER_FRAME.count();
			mainCamera.setSize(mainCameraSize);

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

			b2World_Step(worldId,SEC_PER_TICK.count(),4);

			b2Vec2 newPosition = b2Body_GetPosition(bodyId);
			
			sprite.setPosition({newPosition.x,newPosition.y});
			timeSinceLastUpdate -= SEC_PER_TICK;
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
