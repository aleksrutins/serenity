using namespace serenity::math;
lua["vec"] = lua.create_table_with(
	"vec2f", static_cast<math::Vec2f(*)(float, float)>(math::vec2),
	"vec2i", static_cast<math::Vec2i(*)(int, int)>(math::vec2),
	"vec2d", static_cast<math::Vec2d(*)(double, double)>(math::vec2),
	"vec2u", static_cast<math::Vec2u(*)(unsigned, unsigned)>(math::vec2),
	"vec3f", static_cast<math::Vec3f(*)(float, float, float)>(math::vec3),
	"vec3i", static_cast<math::Vec3i(*)(int, int, int)>(math::vec3),
	"vec3d", static_cast<math::Vec3d(*)(double, double, double)>(math::vec3),
	"vec3u", static_cast<math::Vec3u(*)(unsigned, unsigned, unsigned)>(math::vec3),
	"vec4f", static_cast<math::Vec4f(*)(float, float, float, float)>(math::vec4),
	"vec4i", static_cast<math::Vec4i(*)(int, int, int, int)>(math::vec4),
	"vec4d", static_cast<math::Vec4d(*)(double, double, double, double)>(math::vec4),
	"vec4u", static_cast<math::Vec4u(*)(unsigned, unsigned, unsigned, unsigned)>(math::vec4),

	"x", math::x,
	"y", math::y,
	"z", math::z,
	"t", math::t,

	"add2f", [](Vec2f a, Vec2f b){return a + b;},
	"sub2f", [](Vec2f a, Vec2f b){return a - b;}
);