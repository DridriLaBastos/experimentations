#ifndef RAY_HPP
#define RAY_HPP

#include "util.hpp"

class Ray
{
public:
	Ray(const Point3f origin, const Vec3f direction);
	Point3f operator[](const float t) const;

	Point3f Origin(void) const;
	Vec3f Direction(void) const;

private:
	Point3f mOrigin;
	Vec3f mDirection;
};

#endif