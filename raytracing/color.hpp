//
// Created by Adrien COURNAND on 17/08/2024.
//

#ifndef EXPERIMENTATIONS_COLOR_HPP
#define EXPERIMENTATIONS_COLOR_HPP

#include "simdvec.hpp"

union Color
{
	uint32_t uival;
	struct {
		uint32_t red:8,green:8,blue:8,alpha:8;
	};

	Color(const uint32_t c=0xFF000000): uival(c)
	{}

	Color(const uint8_t r,const uint8_t g,const uint8_t b,const uint8_t a=0xFF):
		red(r),green(g),blue(b),alpha(a)
	{}

	Color(const float r,const float g,const float b,const float a=1.0):
		red(static_cast<uint8_t>(r * 255.0)),
		green(static_cast<uint8_t>(g * 255.0)),
		blue(static_cast<uint8_t>(b * 255.0)),
		alpha(static_cast<uint8_t>(a * 255.0))
	{}
};

namespace Colors
{
	const Color RED   (1.f,0.f,0.f);
	const Color GREEN (0.f,1.f,0.f);
	const Color BLUE  (0.f,0.f,1.f);

	const Color MAGENTA = RED.uival + BLUE.uival;
	const Color CYAN = GREEN.uival + BLUE.uival;
	const Color YELLOW = RED.uival + GREEN.uival;
}

#endif //EXPERIMENTATIONS_COLOR_HPP
