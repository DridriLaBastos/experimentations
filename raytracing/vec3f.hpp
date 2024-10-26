#ifndef VEC3F_HPP
#define VEC3F_HPP

union Vec3f
{
	struct
	{
		float x,y,z;
	};

	struct
	{
		float r,g,b;
	};

	Vec3f(const float _x = 0, const float _y = 0, const float _z = 0);

	float Length(void) const;

	Vec3f& operator*= (const float x);
	Vec3f& operator/= (const float x);

	Vec3f& operator+= (const Vec3f& v);
	Vec3f& operator-= (const Vec3f& v);

	static Vec3f Normalize(const Vec3f& v);
};

using Point3f = Vec3f;
using Color3f = Vec3f;

Vec3f operator* (const float x,const Vec3f& v);
Vec3f operator/ (const Vec3f& v, const float x);

Vec3f operator+ (const Vec3f& a, const Vec3f& b);
Vec3f operator- (const Vec3f& a, const Vec3f& b);

namespace Colors
{
	const Color3f BLACK {.0f,.0f,.0f};
	const Color3f WHITE {1.f,1.f,1.f};
	const Color3f RED   {1.f,0.f,0.f};
	const Color3f GREEN {0.f,1.f,0.f};
	const Color3f BLUE  {0.f,0.f,1.f};

	const Color3f MAGENTA = RED + BLUE;
	const Color3f CYAN = GREEN + BLUE;
	const Color3f YELLOW = RED + GREEN;
}


#endif
