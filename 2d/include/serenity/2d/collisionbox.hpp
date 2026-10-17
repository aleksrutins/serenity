#pragma once
#include <serenity/core.hpp>
#include <SDL3/SDL.h>
#include <optional>

namespace serenity {
namespace twod {

class [[sup]] CollisionBox : public serenity::Component
{

	std::optional<serenity::math::Vec2f> _size;
public:

	enum {
		Solid,
		Movable,
		PassThrough,
		DoNotAvoid
	} mode;

	CollisionBox(serenity::Entity *e, decltype(mode) mode, decltype(_size) size = std::nullopt) : Component(e), _size(size), mode(mode) {}

	void init(Sup *s);
	void update(Sup *s);

	auto rect() -> serenity::math::Vec4f;

	bool hasCollision();
	bool overlaps(serenity::math::Vec2f);
	bool overlaps(CollisionBox*);
};

}
}
