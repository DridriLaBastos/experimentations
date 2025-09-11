#include <optional>

#include "SFML/Graphics.hpp"

int main(int argc, char const *argv[])
{
	sf::RenderWindow window(sf::VideoMode{{640, 480}}, "Evolve");

	while (window.isOpen())
	{
		while (const std::optional e = window.pollEvent())
		{
			if (e->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		window.clear();
		window.display();
	}

	return 0;
}
