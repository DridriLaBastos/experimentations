#include "graphics/ray.hpp"

Ray::Ray(const Point3f origin, const Vec3f direction): mOrigin(origin), mDirection(direction)
{
}

Point3f Ray::Origin(void) const { return mOrigin; }
Vec3f Ray::Direction(void) const { return mDirection; }

Point3f Ray::operator[](const float t) const
{
	return mOrigin + t * mDirection;
}
