#ifndef VEC3F_HPP
#define VEC3F_HPP

struct Vec3f
{
	float x,y,z;

	Vec3f& operator*= (const float x);

	Vec3f& operator+= (const Vec3f& v);
	Vec3f& operator-= (const Vec3f& v);
};

using Point3f = Vec3f;

Vec3f operator* (const float x,const Vec3f& v);
Vec3f operator+ (const Vec3f& a, const Vec3f& b);
Vec3f operator- (const Vec3f& a, const Vec3f& b);


#endif
