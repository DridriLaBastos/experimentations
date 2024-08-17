//
// Created by Adrien COURNAND on 17/08/2024.
//

#include <fstream>

#include <cassert>

#include "ppm.hpp"

PPMUtil::PPMUtil(const size_t w, const size_t h): height(h), width(w), size(h*w), m_buffer(new UVec4 [h*w])
{
}

UVec4 &PPMUtil::at(const size_t x, const size_t y) {
	const size_t linearCoordinate = y*width+x;
	return m_buffer.get()[linearCoordinate];
}

const UVec4& PPMUtil::at(const size_t x, const size_t y) const {
	const size_t linearCoordinate = y*width+x;
	return m_buffer.get()[linearCoordinate];
}

void PPMUtil::write(const std::filesystem::path& path, const uint16_t maxColorValue)
{
	std::ofstream output (path);

	assert(output.is_open());

	output << "P3\n";
	output << width << ' ' << height << '\n';
	output << maxColorValue << '\n';

	for (size_t y = 0; y < height; y += 1)
	{
		for (size_t x = 0; x < width; x += 1)
		{
			const size_t linearIndex = y*width + x;
			UVec4 outputColor = m_buffer.get()[linearIndex];
			outputColor.val *= maxColorValue;
			outputColor.Round();
			output << (int)outputColor.color.r << ' ' << (int)outputColor.color.g << ' ' << (int)outputColor.color.b << ' ';
		}

		output << '\n';
	}
}

