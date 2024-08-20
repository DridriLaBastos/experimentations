//
// Created by Adrien COURNAND on 17/08/2024.
//

#ifndef EXPERIMENTATIONS_COLOR_HPP
#define EXPERIMENTATIONS_COLOR_HPP

#include "simdvec.hpp"

using Color = UVec4;

namespace Colors
{
	const Color RED   {1.0};
	const Color GREEN {0.0, 1.0};
	const Color BLUE  {0.0, 0.0, 1.0};

	const Color MAGENTA = RED + BLUE;
	const Color CYAN = GREEN + BLUE;
	const Color YELLOW = RED + GREEN;
}

#endif //EXPERIMENTATIONS_COLOR_HPP
