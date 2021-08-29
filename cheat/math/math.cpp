#include "math.hpp"
#include "../includes.hpp"

void math::sin_cos( float radians , float* sine , float* cosine ) {
	*sine = std::sinf( radians );
	*cosine = std::cosf( radians );
}

float math::rad2deg( float rad ) {
	float result = rad * ( 180.0f / pi );
	return result;
}

float math::deg2rad( float deg ) {
	float result = deg * ( pi / 180.0f );
	return result;
}