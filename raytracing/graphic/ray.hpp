#ifndef RAY_HPP
#define RAY_HPP

#include "math.hpp"

class Ray
{
	public:
		Ray(const Point4f& o, const Vec4f& dir);

		Point4f operator[] (const float t)
		{
			return origin + t*direction;
		}

	public:
		const Point4f origin;
		const Vec4f direction;
};

#endif