#include "geometry/sphere.hpp"

Sphere::Sphere(const Point3f& center, const float radius): mCenter(center), mRadius(fmax(0,radius))
{}

bool Sphere::Hit(const Ray& r, const float tMin, const float tMax, HitRecord& rec) const
{
	const Vec3f oc = mCenter - r.Origin();
	const float a = glm::length2(r.Direction());
	const float h = glm::dot(r.Direction(),oc);
	const float c = glm::length2(oc) - mRadius*mRadius;
	const float discriminant = h*h-a*c;

	if (discriminant < 0)
	{ return false; }

	const float sqrtDiscriminant = sqrtf(discriminant);
	float root = (h - sqrtDiscriminant) / a;

	if ((root < tMin) || (root >= tMax))
	{
		root = (h + sqrtDiscriminant) / a;
		if ((root < tMin) || (root >= tMax))
		{
			return false;
		}
	}

	rec.t = root;
	rec.p = r[root];
	rec.normal = (rec.p - mCenter) / mRadius;

	return true;
}