#include <iostream>

#include "SFML/Graphics.hpp"

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;
constexpr unsigned int WORLD_CENTER_X = WINDOW_WIDTH / 2;
constexpr unsigned int WORLD_CENTER_Y = WINDOW_HEIGHT / 4;
constexpr float PIXEL_PRE_METER = 5.f;

using Clock = std::chrono::high_resolution_clock;

union Color {
	uint32_t uival;
	struct {
		uint32_t alpha:8, blue:8, green:8, red:8;
	};

	constexpr operator uint32_t() const { return uival; }
};

struct Colors {
	static constexpr Color White = {0xFFFFFFFF};
	static constexpr Color Black = {0x000000FF};
	static constexpr Color Red = {0xFF0000FF};
	static constexpr Color Green = {0xFF00FF00};
	static constexpr Color Blue = {0xFFFF0000};
	static constexpr Color Yellow = {Red | Green};
	static constexpr Color Magenta = {Red | Blue};
	static constexpr Color Cyan = {Green | Blue};
	static constexpr Color Gray = {0xFF808080};
	static constexpr Color DarkGray = {0xFF404040};
	static constexpr Color LightGray = {0xFFC0C0C0};

};

struct RenderData {
	unsigned int height;
	unsigned int width;
	std::unique_ptr<Color> buffer;

	RenderData(const unsigned int w, const unsigned int h) : height(h), width(w), buffer(new Color[w*h]) {}
};

static void PutRectangle(RenderData& renderData, const float x, const float y, const float w, const float h, const Color color)
{
	const int x0 = std::max(0, (int)x);
	const int x1 = std::min((int)(x + w), (int)renderData.width);
	const int y0 = std::max(0, (int)y);
	const int y1 = std::min((int)(y + h), (int)renderData.height);

	for (int j = y0; j < y1; j++)
	{
		for (int i = x0; i < x1; i++)
		{
			renderData.buffer.get()[(j * renderData.width) + i] = color;
		}
	}
}

#define RESOURCE_PATH(path) RESOURCE_FOLDER "/" path

int main(int argc, char const *argv[])
{
	RenderData renderData(720,480);

	sf::RenderWindow window(sf::VideoMode({680,400}),"Evolve");
	window.setFramerateLimit(60);

	sf::Font font (RESOURCE_PATH("fonts/LTKaraoke-SemiBold.ttf"));
	sf::Text text (font);
	text.setPosition({10,10});
	text.setCharacterSize(15);
	text.setFillColor(sf::Color::Red);

	// PutRectangle(renderData, 0, 100, 100, 100, Colors::Green);
	// PutRectangle(renderData, 0, 200, 100, 100, Colors::Blue);

	Color color = Colors::Black;

	PutRectangle(renderData, 0, 0, renderData.width, renderData.height, Colors::LightGray);

	sf::View mainCamera (window.getView());
	window.setView(mainCamera);

	sf::Texture particleTexture(RESOURCE_PATH("textures/particle.jpeg"));

	constexpr unsigned int N = 50000;
	std::vector<sf::Sprite> particleSprites (N, sf::Sprite(particleTexture));

	for (size_t i = 0; i < N; i+=1)
	{
		const unsigned int x = i % (window.getSize().x/2);
		const unsigned int y = i / window.getSize().y;
		particleSprites[i].setPosition({(float)x,(float)y});
	}

	auto frameTimeBegin = Clock::now();
	auto frameTimeEnd = Clock::now();
	auto begin = Clock::now();
	unsigned int frameCount = 0;
	std::chrono::duration<double> elapsed_seconds (0);
	std::chrono::duration<double,std::milli> averageFrameTime (0);

	while (window.isOpen())
	{
		frameCount += 1;
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			else if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
				switch (key->code) {
					case sf::Keyboard::Key::D:
						mainCamera.move({10,0});
					break;

					case sf::Keyboard::Key::A:
						mainCamera.zoom(0.5);
					break;

					case sf::Keyboard::Key::E:
						mainCamera.zoom(1.5);
					break;

					default:
						break;
				}
			}
		}

		window.clear();
		window.setView(mainCamera);
		std::for_each(particleSprites.begin(), particleSprites.end(), [&window](sf::Sprite& sprite) {
			window.draw(sprite);
		});
		window.setView(window.getDefaultView());
		window.draw(text);
		window.display();

		const auto now = Clock::now();

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
		averageFrameTime += frameTimeEnd - frameTimeBegin;
		frameTimeBegin = frameTimeEnd;
	}

	return 0;
}
