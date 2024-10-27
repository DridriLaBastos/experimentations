#include "geometry/hittable.hpp"

void HitRecord::SetFaceNormal(const Ray& r, const Vec3f& outwardNormal)
{
	frontFace = glm::dot(r.Direction(),outwardNormal) < 0;
	normal = frontFace ? outwardNormal : -outwardNormal;
}
