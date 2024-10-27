#include "geometry/hittable_list.hpp"

bool HittableList::Hit(const Ray& r, const float tMin, const float tMax, HitRecord& rec) const
{
	HitRecord tempRecord;
	bool hasHit = false;
	float closest = tMax;

	for (const Hittable* hittable : mHittableList)
	{
		if (hittable->Hit(r,tMin,closest,tempRecord))
		{
			hasHit = true;
			closest = tempRecord.t;
			rec = tempRecord;
		}
	}

	return hasHit;
}

void HittableList::Add (Hittable* hittable)
{
	mHittableList.emplace_back(hittable);
}