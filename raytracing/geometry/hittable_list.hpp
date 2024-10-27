#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <vector>

#include "geometry/hittable.hpp"

class HittableList : public Hittable
{
public:
	bool Hit(const Ray& r, const float tMin, const float tMax, HitRecord& rec) const final;

	void Add (Hittable* hittable);

private:
	std::vector<Hittable*> mHittableList;
};

#endif
