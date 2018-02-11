
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <random>

#ifndef _UTILS_HPP_
#define _UTILS_HPP_

// edited from
// https://en.wikipedia.org/wiki/Fast_inverse_square_root
float Q_rsqrt(float number);

// I heard that floats are much faster.
class Vec3 {
public:
	float x;
	float y;
	float z;

	Vec3(double _x, double _y, double _z)
		: x(_x), y(_y), z(_z) {};

	Vec3() : Vec3(0.0, 0.0, 0.0) {};

	Vec3 operator*(float mult){
		return Vec3(x * mult, y * mult, z * mult);
	}

	Vec3 operator-(Vec3 v) {
		return Vec3(x -=v.x, y - v.y, z - v.z);
	}

	inline void normalize() {
		float invsq = Q_rsqrt(x*x + y*y + z*z);
		x *= invsq;
		y *= invsq;
		z *= invsq;
	}

	inline float dot(Vec3 v) {
		return x*v.x + y*v.y + z*v.z;
	}

	inline static Vec3 cross(Vec3 a, Vec3 b) {
		Vec3 ans;
        ans.x = a.y * b.z - a.z * b.y;
        ans.y = a.z * b.x - a.x * b.z;
        ans.z = a.x * b.y - a.y * b.x;
        return ans;
	}

	inline static Vec3 project(Vec3 n, Vec3 v) {
		// get dist from v to plane n
		// subt dist * n from v
		return v - n * v.dot(n);
	}
};

inline uint32_t lerpColor(uint32_t color1, uint32_t color2, double lerp);
inline uint32_t multiplyColor(uint32_t color, double r, double g, double b);
inline uint32_t shiftColor(uint32_t color, int r, int g, int b);

inline double clockToMilliseconds(clock_t ticks) {
    return (ticks/(double)CLOCKS_PER_SEC) * 1000.0;
}

#endif