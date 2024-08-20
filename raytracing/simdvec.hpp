//
// Created by Adrien COURNAND on 17/08/2024.
//

#ifndef EXPERIMENTATIONS_VEC_HPP
#define EXPERIMENTATIONS_VEC_HPP

#include <arm_neon.h>

union UVec4
{
	float32x4_t val;

	struct {
		float32_t r,g,b,a;
	} color;

	struct {
		float32_t x,y,z,t;
	} pos;

	explicit UVec4(const float _x = .0f, const float _y = .0f, const float _z = .0f, const float _t = .0f): val{} {
		pos.x = _x;
		pos.y = _y;
		pos.z = _z;
		pos.t = _t;
	}

	void Round(void)
	{
		val = vrndaq_f32(val);
	}
};

inline UVec4 operator+ (const UVec4& v1, const UVec4& v2)
{
	UVec4 ret;
	ret.val = v1.val + v2.val;
	return ret;
}

inline UVec4 operator* (const float f, const UVec4& v)
{
	UVec4 ret;
	ret.val = f*v.val;
	return ret;
}

#endif //EXPERIMENTATIONS_VEC_HPP
