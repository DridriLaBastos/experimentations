#ifndef UTIL_HPP
#define UTIL_HPP

#include "glm/glm.hpp"

using Vec3f   = glm::vec3;
using Point3f = glm::vec3;
using Color3f = glm::vec3;

namespace Colors
{
	const Color3f BLACK {.0f,.0f,.0f};
	const Color3f WHITE {1.f,1.f,1.f};
	const Color3f RED   {1.f,0.f,0.f};
	const Color3f GREEN {0.f,1.f,0.f};
	const Color3f BLUE  {0.f,0.f,1.f};

	const Color3f MAGENTA = RED + BLUE;
	const Color3f CYAN = GREEN + BLUE;
	const Color3f YELLOW = RED + GREEN;
}

#endif