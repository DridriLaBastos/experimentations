#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "graphics/ray.hpp"
#include "util.hpp"

struct HitRecord
{
	Point3f p;
	Vec3f normal;
	float t;
	bool frontFace;

	void SetFaceNormal(const Ray& r, const Vec3f& outwardNormal);
};

class Hittable
{
public:
	virtual ~Hittable() = default;
	virtual bool Hit(const Ray& r, const float tMin, const float tMax, HitRecord& rec) const = 0;
};

#endif