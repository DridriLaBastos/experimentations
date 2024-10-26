#include <cmath>

#include "vec3f.hpp"

Vec3f::Vec3f(const float _x, const float _y, const float _z): x(_x),y(_y),z(_z)
{}

float Vec3f::Length(void) const
{
	return std::sqrtf(x*x + y*y + z*z);
}

Vec3f Vec3f::Normalize(const Vec3f& v)
{
	return v / v.Length();
}

Vec3f& Vec3f::operator*= (const float _x)
{
	this->x *= _x;
	this->y *= _x;
	this->z *= _x;

	return *this;
}

Vec3f& Vec3f::operator/=(const float _x)
{
	this->x /= _x;
	this->y /= _x;
	this->z /= _x;

	return *this;
}

Vec3f& Vec3f::operator+= (const Vec3f& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;

	return *this;
}

Vec3f& Vec3f::operator-= (const Vec3f& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;

	return *this;
}



Vec3f operator* (const float x,const Vec3f& v)
{
	Vec3f ret (v);
	ret *= x;
	return ret;
}

Vec3f operator/ (const Vec3f& v, const float x)
{
	Vec3f ret (v);
	ret /= x;
	return ret;
}


Vec3f operator+ (const Vec3f& a, const Vec3f& b)
{
	Vec3f ret (a);
	ret += b;
	return ret;
}

Vec3f operator- (const Vec3f& a, const Vec3f& b)
{
	Vec3f ret (a);
	ret -= b;
	return ret;
}
