#include <iostream>

#include "SFML/Graphics.hpp"

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;

#define RESOURCE_PATH(path) RESOURCE_FOLDER "/" path

using Clock = std::chrono::high_resolution_clock;

int main(int argc, char const *argv[])
{
	sf::RenderWindow window(sf::VideoMode({720,480}),"Evolve");
	window.setFramerateLimit(60);

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
	auto begin = Clock::now();
	auto lastEvent = Clock::now();
	unsigned int frameCount = 0;
	std::chrono::duration<double,std::milli> averageFrameTime (0);
	std::chrono::duration<double> currentFrameTime (0);

	float zoomAccelerationFactor = 300.0;
	float zoomAcceleration = 0;
	float zoomVelocity = 0.0;
	float maxZoomVelocity = 500.0;
	float zoomAttenuation = 0.2;

	while (window.isOpen())
	{
		frameCount += 1;
		const auto now = Clock::now();
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			else if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
				switch (key->code) {
					case sf::Keyboard::Key::A:
						zoomAcceleration = zoomAccelerationFactor;
					break;

					case sf::Keyboard::Key::E:
						zoomAcceleration = -zoomAccelerationFactor;
					break;

					default:
						break;
				}
			}
			else if (auto* key = event->getIf<sf::Event::KeyReleased>())
			{
				switch (key->code)
				{
					case sf::Keyboard::Key::A:
					case sf::Keyboard::Key::E:
						zoomAcceleration = 0;
					break;

					default:
						break;
				}
			}
		}

		zoomVelocity = std::min(zoomVelocity,maxZoomVelocity);

		sf::Vector2f mainCameraSize = mainCamera.getSize();
		zoomVelocity += zoomAcceleration * currentFrameTime.count();
		mainCameraSize.x += zoomVelocity * currentFrameTime.count();
		mainCameraSize.y += zoomVelocity * currentFrameTime.count();
		mainCamera.setSize(mainCameraSize);

		//If the sign are opposed, then the acceleration is against the movement and we want to attenuate the zoom
		const bool accelerationOpposed = (zoomAcceleration / zoomVelocity) < 0;
		const bool notAccelerating = std::abs(zoomAcceleration) < 0.01;
		const bool attenuateZoom = accelerationOpposed || notAccelerating;

		if (attenuateZoom)
			zoomVelocity += -zoomVelocity*5*currentFrameTime.count();

		printf("Zoom Velocity: %.3f\n", zoomVelocity);

		window.clear(sf::Color::Magenta);
		window.setView(mainCamera);
		window.draw(sprite);
		window.setView(window.getDefaultView());
		window.draw(text);
		window.display();

		if (std::chrono::duration_cast<std::chrono::seconds>(now - begin).count() >= 1)
		{
			char buffer [64];
			snprintf(buffer,sizeof(buffer),"FPS %d (%.3fms)", frameCount, averageFrameTime.count() / frameCount);
			text.setString(buffer);

			frameCount = 0;
			averageFrameTime = std::chrono::duration<double, std::milli>::zero();
			begin = now;
		}

		frameTimeEnd = now;
		currentFrameTime = frameTimeEnd - frameTimeBegin;
		averageFrameTime += currentFrameTime;
		frameTimeBegin = frameTimeEnd;
	}

	return 0;
}
