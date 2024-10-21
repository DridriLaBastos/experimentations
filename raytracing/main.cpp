//
// Created by Adrien COURNAND on 17/08/2024.
//

#include <cstdio>
#include <cstdlib>

#include "SFML/Graphics.hpp"

#include "ppm.hpp"
#include "color.hpp"

#include "util/module/module.hpp"

constexpr size_t IMAGE_WIDTH = 720;
constexpr size_t IMAGE_HEIGHT = 480;

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

	sf::RenderWindow window (sf::VideoMode(640,480),"Raytracing");

	while (window.isOpen())
	{
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

		window.clear();
		window.display();
	}

	return EXIT_SUCCESS;
}