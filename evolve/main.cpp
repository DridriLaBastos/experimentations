#include <thread>
#include <format>
#include <chrono>
#include <optional>
#include <iostream>

#include "SFML/Graphics.hpp"

using Clock = std::chrono::high_resolution_clock;
using TimePoint = Clock::time_point;

using Seconds = std::chrono::duration<float>;
using Milliseconds = std::chrono::duration<float, std::milli>;

static constexpr float FRAME_PER_SECONDS = 25.f;

int main(int argc, char const *argv[])
{
	sf::RenderWindow window(sf::VideoMode{{640, 480}}, "Evolve");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(FRAME_PER_SECONDS);

	sf::Font font("/Users/adrien/Documents/Informatique/experimentations/evolve/evolve/resources/fonts/LTKaraoke-SemiBold.ttf");

	sf::Text t(font);

	unsigned int fps = 0;
	Milliseconds elapsed = Seconds::zero();
	TimePoint begin = Clock::now();

	while (window.isOpen())
	{
		const TimePoint now = Clock::now();
		elapsed += now - begin;
		begin = now;
		while (const std::optional e = window.pollEvent())
		{
			if (e->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		window.clear();
		window.draw(t);
		window.display();
		fps += 1;

		if (elapsed >= Seconds(1))
		{
			t.setString(std::format("FPS {}\nTPS {:.2f}\nelapsed {:.2f}ms", fps, elapsed.count() / fps, elapsed.count()));
			fps = 0;
			elapsed -= Seconds(1);
		}
	}

	return 0;
}
