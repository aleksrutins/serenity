#pragma once
#include <algorithm>
#include <iostream>
#include "../util/_vec.internal.hpp"

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

	VEC_CMPOP(==, &&)
	VEC_CMPOP(>,  ||)
	VEC_CMPOP(<,  ||)

	VEC_BINOP_V(+)
	VEC_BINOP_N(+)
	VEC_BINOP_V(-)
	VEC_BINOP_N(-)
	VEC_UNOP   (+)
	VEC_UNOP   (-)

	VEC_BINOP_V(*)
	VEC_BINOP_N(*)
	VEC_BINOP_V(/)
	VEC_BINOP_N(/)

	bool isZero() {
		using namespace std;
		return all_of(begin(components), end(components), [](T v){return v == 0;});
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

VEC_BINOP_S_SIMPLE(+)
VEC_BINOP_S_ELWISE(-)
VEC_BINOP_S_SIMPLE(*)
VEC_BINOP_S_ELWISE(/)

}
}

template <unsigned int len, typename T>
std::ostream &std::operator<<(std::ostream &os, serenity::math::Vec<len, T> v) {
	os << "<";
	for(unsigned int i = 0; i < len; i++) {
		os << v[i];
		if(i < len-1) os << ", ";
	}
	return os << ">";
}

#include "../util/_vec.internal.un.hpp"