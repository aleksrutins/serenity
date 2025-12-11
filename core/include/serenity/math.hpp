#pragma once
#include <algorithm>

namespace serenity {
namespace math {

static const int x = 0;
static const int y = 1;
static const int z = 2;
static const int t = 3;

template <unsigned int len, typename T>
struct Vec {

	T components[len];

	T &operator[](size_t i) {
		return components[i];
	}

	Vec<len, T> operator+(Vec<len, T> other) {
		Vec<len, T> res;
		for(unsigned int i = 0; i < len; i++) {
			res[i] = components[i] + other[i];
		}
		return res;
	}

	Vec<len, T> operator-(Vec<len, T> other) {
		Vec<len, T> res;
		for(unsigned int i = 0; i < len; i++) {
			res[i] = components[i] - other[i];
		}
		return res;
	}

	template <typename C>
	operator Vec<len, C>() {
		Vec<len, C> res;
		for(unsigned int i = 0; i < len; i++) {
			res[i] = static_cast<C>(components[i]);
		}
		return res;
	}

	Vec() : components{0} {};
	Vec(T c[len]) {
		std::copy(components, components + len, c);
	};
};


template <typename T>
using Vec2 = Vec<2, T>;
template <typename T>
inline Vec2<T> vec2(T x, T y) {
	Vec2<T> res;
	res[math::x] = x;
	res[math::y] = y;
	return res;
}

template <typename T>
using Vec3 = Vec<3, T>;
template <typename T>
inline Vec3<T> vec3(T x, T y, T z) {
	Vec3<T> res;
	res[math::x] = x;
	res[math::y] = y;
	res[math::z] = z;
	return res;
}

template <typename T>
using Vec4 = Vec<4, T>;
template <typename T>
inline Vec4<T> vec4(T x, T y, T z, T t) {
	Vec4<T> res;
	res[math::x] = x;
	res[math::y] = y;
	res[math::z] = z;
	res[math::t] = t;
	return res;
}

template <typename T>
inline auto lerp(T a, T b, double t) -> T {
    return a + t * (b - a);
}

}
}