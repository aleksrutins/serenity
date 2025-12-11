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

	Vec<len, T> operator+(Vec<len, T> other) const {
		Vec<len, T> res;
		for(unsigned int i = 0; i < len; i++) {
			res[i] = components[i] + other[i];
		}
		return res;
	}

	Vec<len, T> operator+(T other) const {
		Vec<len, T> res;
		for(unsigned int i = 0; i < len; i++) {
			res[i] = components[i] + other;
		}
		return res;
	}

	Vec<len, T> operator-(Vec<len, T> other) const {
		Vec<len, T> res;
		for(unsigned int i = 0; i < len; i++) {
			res[i] = components[i] - other[i];
		}
		return res;
	}

	Vec<len, T> operator-(T other) const {
		Vec<len, T> res;
		for(unsigned int i = 0; i < len; i++) {
			res[i] = components[i] - other;
		}
		return res;
	}

	Vec<len, T> operator-() const {
		Vec<len, T> res;
		for(unsigned int i = 0; i < len; i++) {
			res[i] = 0 - components[i];
		}
		return res;
	}

	Vec<len, T> operator*(T coeff) const {
		Vec<len, T> res;
		for(unsigned int i = 0; i < len; i++) {
			res[i] = components[i] * coeff;
		}
		return res;
	}

	Vec<len, T> operator/(T divisor) const {
		Vec<len, T> res;
		for(unsigned int i = 0; i < len; i++) {
			res[i] = components[i] / divisor;
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

template <unsigned int len, typename E>
inline Vec<len, E> operator+(E other, Vec<len, E> self) {
	return self + other;
}

template <unsigned int len, typename E>
inline Vec<len, E> operator-(E other, Vec<len, E> self) {
	return other + (-self);
}

template <unsigned int len, typename E>
inline Vec<len, E> operator*(E coeff, Vec<len, E> self) {
	return self * coeff;
}

template <unsigned int len, typename E>
inline Vec<len, E> operator/(E dividend, Vec<len, E> self) {
	Vec<len, E> res;
	for(unsigned int i = 0; i < len; i++) {
		res[i] = dividend / self[i];
	}
	return res;
}

#define __DEFINE_VEC_TYPES__(n) template <typename T> using Vec##n = Vec<n, T>; \
	using Vec##n##i = Vec##n<int>; \
	using Vec##n##f = Vec##n<float>; \
	using Vec##n##d = Vec##n<double>; \
	using Vec##n##u = Vec##n<unsigned int>;

__DEFINE_VEC_TYPES__(2)
__DEFINE_VEC_TYPES__(3)
__DEFINE_VEC_TYPES__(4)

template <typename T>
inline Vec2<T> vec2(T x, T y) {
	Vec2<T> res;
	res[math::x] = x;
	res[math::y] = y;
	return res;
}

template <typename T>
inline Vec3<T> vec3(T x, T y, T z) {
	Vec3<T> res;
	res[math::x] = x;
	res[math::y] = y;
	res[math::z] = z;
	return res;
}

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
inline auto lerp(T a, T b, float t) -> T {
    return a + t * (b - a);
}

}
}