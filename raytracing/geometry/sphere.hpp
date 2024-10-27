#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "geometry/hittable.hpp"

class Sphere : public Hittable
{
public:
	Sphere(const Point3f& center, const float radius);

	bool Hit(const Ray& r, const float tMin, const float tMax, HitRecord& rec) const final;

	Point3f mCenter;
	float mRadius;
};

#endif