//
// Created by Adrien COURNAND on 17/08/2024.
//

#include <cstdio>
#include <cstdlib>

#include "ppm.hpp"
#include "colors.hpp"

constexpr size_t IMAGE_WIDTH = 720;
constexpr size_t IMAGE_HEIGHT = 480;

int main(void)
{
	PPMUtil ppm (IMAGE_WIDTH,IMAGE_HEIGHT);

	for (size_t y = 0; y < ppm.height; y += 1)
	{
		for (size_t x = 0; x < ppm.width; x += 1)
		{
			ppm.at(x,y) = Colors::BLUE;
		}
	}

	ppm.write("test.ppm");

	return EXIT_SUCCESS;
}