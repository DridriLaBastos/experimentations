#include "vec3f.hpp"

Vec3f& Vec3f::operator*= (const float x)
{
	this->x *= x;
	this->y *= y;
	this->z *= z;

	return *this;
}

Vec3f& Vec3f::operator+= (const Vec3f& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;

	return *this;
}

Vec3f operator* (const float x,const Vec3f& v)
{
	Vec3f ret (v);
	ret *= x;
	return ret;
}

Vec3f operator+ (const Vec3f& a, const Vec3f& b)
{
	Vec3f ret (a);
	ret += b;
	return ret;
}
