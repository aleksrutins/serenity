#pragma once
#include <serenity/core.hpp>

namespace serenity {
namespace twod {

class Transform;
class Spritesheet;

class [[sup]] Sprite : public serenity::Component
{
	Spritesheet *_sheet;

	serenity::math::Vec2f _pos;
	serenity::math::Vec2f _size;

	Sprite(serenity::Entity*, Spritesheet *, serenity::math::Vec2f, serenity::math::Vec2f);

	friend class Spritesheet;

public:

	static Sprite *load(serenity::Entity*, const char *imageName, serenity::math::Vec2f size);

	void update(Sup *);

	inline auto size() -> decltype(_size) {return _size;}

	inline auto box() -> serenity::math::Vec4f {
		return serenity::math::vec4(getComponent<Transform>()->position(), size());
	}
};

}
}
